#include <stdio.h>
#include <hash/sha2.h>
const char PressCommand[] = "Press Command - >";
uint32_t Base_address,key_status,cmd_char_num = 17,line_char_num = 17,line_num = 0,command_base = 0,command_len = 0;
char key,command[127] = {0};
static __attribute__((optimize("O2","no-inline"))) void nocommand(){
    uint8_t color1 = color;
    color = 0x04;
    printf("Command not Found!");
    printf("Press 'help' or 'h' to get more information");
    color = color1;
    return;
}
static __attribute__((optimize("Ofast","no-inline"))) uint8_t cmd_calculate(){
    uint8_t command_check_flag = 0;
    uint32_t last_char = 0;
    for(int i = command_base;i < 127;i++){
            if(command[i] != 0 && command[i] != 0x20){
                if(command_check_flag == 0){
                    command_base = i;
                    command_check_flag = 1;
                }
                last_char = i;
            }
    }
    command_len = last_char + 1 - command_base;
    if(command_check_flag == 0){
        return 0;
    }
    return 1;
}
static __attribute__((optimize("Ofast","no-inline"))) void color_error(){
    printf("Unknown color");
    test_scolling();
    printf("color format :    XX");
    printf("                 -- --");
    printf("                 ^  ^");
    printf("                0~F 0~F");
    printf("   background color  foreground color");
    return;
}
static __attribute__((optimize("Ofast","no-inline"))) uint32_t cmd_code_check(const char s[]){
    uint32_t len = 0;
    while (s[len])++len;
    for(int i = 0;i < len;i++){
        if(command[command_base + i] != s[i]){
            return 0;
        }
    }
    if(command[command_base + len] != 0x00 && command[command_base + len] != 0x20){
        return 0;
    }
    return 1;
}
static __attribute__((optimize("O2","no-inline"))) void code(){
    if(cmd_code_check("help") == 1 || cmd_code_check("h") == 1){
        printf("OpenFerain OS @ Command Shell 1.0.7_19");
        test_scolling();
        printf("color [color] --  set color");
        printf("hash [-algorithm_name] [str] --  Get a string's hash");
        printf("pr [str] --  Print string");
        printf("time [a/y/mo/d/h/mi/s] --  Get time all/year/month/day/hour/minute/second");
        printf("ver [appname]  --  Get an application's version");
        printf("help/h  --  Get help");
    }
    else if(cmd_code_check("color") == 1){
        command_base = command_base + 6;
        command_len = command_len - 6;
        if(command[command_base] == '0' && (command[command_base + 1] == 'x' || command[command_base + 1] == 'X')){
            command_base = command_base + 2;
            command_len = command_len - 2;
            if(char_to_hex(command[command_base]) == 0x1000 || char_to_hex(command[command_base + 1]) == 0x1000){
                color_error();
            }
            else{
                color = (uint8_t)char_to_hex(command[command_base]) << 4 | (uint8_t)char_to_hex(command[command_base + 1]);
                set_color();
            }
        }
        else if(char_to_hex(command[command_base]) == 0x1000 || char_to_hex(command[command_base + 1]) == 0x1000){
            color_error();
        }
        else{
            color = (uint8_t)char_to_hex(command[command_base]) << 4 | (uint8_t)char_to_hex(command[command_base + 1]);
            set_color();
        }
    }
    else if(cmd_code_check("hash") == 1){
        command_base = command_base + 5;
        command_len = command_len - 5;
        if(cmd_code_check("sha256") == 1){
            command_base = command_base + 7;
            command_len = command_len - 7;
            sha256(&command[command_base]);
        }
        else{
            printf("Unkown algorithm!");
        }
    }
    else if(cmd_code_check("pr") == 1){
        if(command[command_base + 2] == 0x00 || command[command_base + 3] == 0x00){
            printf("Please press string!");
            return;
        }
        command_base = command_base + 3;
        command_len = command_len - 3;
        printf(&command[command_base]);
    }
    else if(cmd_code_check("time") == 1){
        command_base = command_base + 5;
        command_len = command_len - 5;
        if(cmd_code_check("a") == 1){
            print_time(0);
        }else if(cmd_code_check("y") == 1){
            print_time(1);
        }else if(cmd_code_check("mo") == 1){
            print_time(2);
        }else if(cmd_code_check("d") == 1){
            print_time(3);
        }else if(cmd_code_check("h") == 1){
            print_time(4);
        }else if(cmd_code_check("mi") == 1){
            print_time(5);
        }else if(cmd_code_check("s") == 1){
            print_time(6);
        }else{
            printf("Parameter error!");
        }
    }
    else if(cmd_code_check("ver") == 1){
        command_base = command_base + 4;
        command_len = command_len - 4;
        if(cmd_code_check("cmd") == 1 || cmd_code_check("CMD") == 1){
            printf("OpenFerain OS @ Command Shell 1.0.7_19");
        }
        else if(cmd_code_check("os") == 1 || cmd_code_check("OS") == 1){
            printf("OpenFerain OS 26.0.7_19");
        }
        else{
            printf("Unknow application");
        }
    }
    else{
        nocommand();
    }
    return;
}
static __attribute__((optimize("O1","no-inline"))) void enter(){
    uint8_t cmd_check_flags = cmd_calculate();
    if(cmd_check_flags == 0){
        command_base = 0;
        command_len = 0;
        printf(PressCommand);
        cmd_char_num = 17;
        line_char_num = 17;
        line_num = 0;
        command_base = 0;
        command_len = 0;
        return;
    }
    code();
    for(int i = 0;i < 127;i++){
            command[i] = 0;
    }
    printf(PressCommand);
    cmd_char_num = 17;
    line_char_num = 17;
    line_num = 0;
    command_base = 0;
    command_len = 0;
    return;
}
static __attribute__((optimize("O0","no-inline"))) void back_space(){
    if(cmd_char_num <= 17){
        return;
    }
    __asm__ __volatile__ (
        "movb $0x03,%%ah\n"
        "int $0x30\n"
        "movb $0x80,%%ah\n"
        "int $0x30\n"
        :
        : "a"(color)
        : "edi","ebx","edx","memory"
    );
    command[cmd_char_num - 17] = 0x00;
    cmd_char_num = cmd_char_num - 1;
    line_char_num = line_char_num - 1;
    if(line_char_num <= 0){
        video_memory_point = video_memory_point - 0x00000100;
        line_num = line_num - 1;
        line_char_num = line_char_num = 79;
    }
    return;
}
static __attribute__((optimize("O0","no-inline"))) void have_key(){
    __asm__ __volatile__ ("": "=c"(key): :);
    if(key == 0x1C){
        enter();
        key_status = 0x00000000;
        return;
    }
    else if(key == 0x0E){
        back_space();
        key_status = 0x00000000;
        return;
    }
    type(key);
    cmd_char_num = cmd_char_num + 1;
    line_char_num = line_char_num + 1;
    if(line_char_num >= 80){
        test_scolling();
        line_num = line_num + 1;
        line_char_num = 0;
    }
    command[cmd_char_num - 17] = key;
    key_status = 0x00000000;
    return;
}
__attribute__((section(".entry"),naked)) void main_func(){
    video_memory_point = 0x00000000;
    color = 0x07;
    printf("OpenFerain Command Shell @ 1.1.26.7.1");
    test_scolling();
    printf(PressCommand);
    for(;;){
        __asm__ __volatile__ ("" : "=a"(key_status): :"ecx","memory");
        if(key_status == 0x00000001){
            have_key();
        }
    }
    return;
}