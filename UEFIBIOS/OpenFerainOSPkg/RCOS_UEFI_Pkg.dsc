[Defines]
    DSC_SPECIFICATION            =  0x00010006
    PLATFORM_NAME                =  OpenFerainOSPkg
    PLATFORM_GUID                =  854F2D7A-9C1E-4B35-A702-F691CD38EB20
    PLATFORM_VERSION             =  1.0
    SKUID_IDENTIFIER             =  DEFAULT
    TOOL_CHAIN_TAG               =  GCC5
    SUPPORTED_ARCHITECTURES      =  IA32
    BUILD_TARGETS                =  DEBUG
    FIX_LOAD_TOP_MEMORY_ADDRESS  =  0x00007000
[LibraryClasses] 
    UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf 
    UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf 
    BaseLib|MdePkg/Library/BaseLib/BaseLib.inf 
    RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf 
    BaseSynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf 
    BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf 
    PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf 
    UefiLib|MdePkg/Library/UefiLib/UefiLib.inf 
    UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf 
    DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf 
    PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf 
    StackCheckLib|MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf 
    MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
    DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
[Components]
    RCOS_UEFI_Pkg/RCOS_UEFI_Pkg.inf