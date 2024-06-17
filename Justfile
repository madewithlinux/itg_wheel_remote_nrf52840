console-loop SERIAL_TTY:
	#!/bin/bash
	while ! (
		test -a {{ SERIAL_TTY }} && picocom --imap lfcrlf --baud 115200 {{ SERIAL_TTY }}
	); do
		sleep 1s;
	done

remote-console-loop:
	just console-loop /dev/serial/by-id/usb-madewithlinux_itg_wheel_remote_*

receiver-console-loop:
	just console-loop /dev/serial/by-id/usb-madewithlinux_itg_wheel_receiver_*


serials:
	ls --color=always -alh /dev/serial/by-path/ /dev/serial/by-id/

watch-serials:
	watch -n1 --color -- 'ls --color=always -alh /dev/serial/by-path/* /dev/serial/by-id/* /dev/ttyACM*'

upload-receiver:
	#!/bin/bash
	source "$HOME/.platformio/penv/bin/activate"
	pio run -e receiver -t upload

upload-remote:
	#!/bin/bash
	source "$HOME/.platformio/penv/bin/activate"
	pio run -e remote -t upload
