#include <Uefi.h>
#include <Protocol/BlockIo.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
extern void asm();
EFI_STATUS 
EFIAPI 
UefiMain (IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE  *SystemTable)
{
    EFI_STATUS                Status;
    EFI_BLOCK_IO_PROTOCOL     *BlockIo;
    EFI_HANDLE                *Handles;
    UINTN                     HandleCount;
    UINTN                     Index;
    UINTN                     BufferSize;
    EFI_LBA                   StartLba;
    EFI_PHYSICAL_ADDRESS      TargetAddr;
    VOID                      *ReadBuffer;
    UINTN                     PagesCount;
    UINTN                     MemMapKey;
    UINTN                     MemMapSize;
    UINTN                     DescriptorSize;
    UINT32                    DescriptorVersion;
    EFI_MEMORY_DESCRIPTOR     *MemMap;
    UINT8                     *Src, *Dst;
    UINTN                     i;
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiBlockIoProtocolGuid,
                    NULL,
                    &HandleCount,
                    &Handles 
                    );
    if (EFI_ERROR (Status)) {
        Print (L"Failed to locate BlockIo handles: %r\n", Status);
        return Status;
    }
 
    BlockIo = NULL;
    for (Index = 0; Index < HandleCount; Index++) {
        Status = gBS->HandleProtocol (
                        Handles[Index],
                        &gEfiBlockIoProtocolGuid,
                        (VOID **)&BlockIo 
                        );
        if (EFI_ERROR (Status)) continue;
        if (!BlockIo->Media->LogicalPartition &&
            !BlockIo->Media->ReadOnly) {
            break;
        }
        BlockIo = NULL;
    }
    gBS->FreePool (Handles);
 
    if (BlockIo == NULL) {
        Print (L"No suitable physical disk found.\n");
        return EFI_NOT_FOUND;
    }
    Print (
        L"Disk found. Block size: %d bytes, Last LBA: 0x%llx\n",
        BlockIo->Media->BlockSize,
        BlockIo->Media->LastBlock 
        );
    BufferSize = 8 * BlockIo->Media->BlockSize;
    PagesCount = EFI_SIZE_TO_PAGES (BufferSize);
    ReadBuffer = NULL;
    Status = gBS->AllocatePages (
                    AllocateAnyPages,
                    EfiBootServicesData,
                    PagesCount,
                    &TargetAddr 
                    );
    if (EFI_ERROR (Status)) {
        Print (L"Failed to allocate memory for kernel: %r\n", Status);
        return Status;
    }
    ReadBuffer = (VOID *)(UINTN)TargetAddr;
    StartLba = 1;
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        BlockIo->Media->MediaId,
                        StartLba,
                        BufferSize,
                        ReadBuffer 
                        );
    if (EFI_ERROR (Status)) {
        Print (L"ReadBlocks failed: %r\n", Status);
        gBS->FreePages (TargetAddr, PagesCount);
        return Status;
    }
    MemMapSize = 0;
    MemMap = NULL;
    Status = gBS->GetMemoryMap (&MemMapSize, MemMap, &MemMapKey,
                                &DescriptorSize, &DescriptorVersion);
    if (Status == EFI_BUFFER_TOO_SMALL) {
        MemMap = AllocatePool (MemMapSize);
        if (MemMap != NULL) {
            Status = gBS->GetMemoryMap (&MemMapSize, MemMap, &MemMapKey,
                                        &DescriptorSize, &DescriptorVersion);
        } else {
            Status = EFI_OUT_OF_RESOURCES;
        }
    }
    if (EFI_ERROR (Status)) {
        Print (L"GetMemoryMap failed: %r\n", Status);
        gBS->FreePages (TargetAddr, PagesCount);
        if (MemMap != NULL) FreePool (MemMap);
        return Status;
    }
 
    Status = gBS->ExitBootServices (ImageHandle, MemMapKey);
    if (EFI_ERROR (Status)) {
        Print (L"ExitBootServices failed: %r\n", Status);
        gBS->FreePages (TargetAddr, PagesCount);
        FreePool (MemMap);
        return Status;
    }
    asm();
    return EFI_SUCCESS;
}