#.type helper %function
.global helper
.global DebugHandler

helper:
    push   %rbp
    mov    %rsp,%rbp
    sub    $0x20,%rsp

    mov    %rsp,%rdi
    addq   $0x28, %rdi
	  callq  DebugHandler

    add    $0x20,%rsp
    pop    %rbp
	  retq