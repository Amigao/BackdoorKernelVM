cmd_/home/vboxuser/backdoor/modules.order := {   echo /home/vboxuser/backdoor/backdoor.ko; :; } | awk '!x[$$0]++' - > /home/vboxuser/backdoor/modules.order
