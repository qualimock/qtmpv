#!/bin/bash


function main {
	packages=("curl", "mpv")

	for package in "${packages[@]}"; do
		if sudo dpkg -s "$package" >/dev/null 2>&1; then
			sudo apt install "$package"
		fi
	done

	sudo mkdir /etc/qtmpv
	sudo touch /etc/qtmpv/token.txt
	sudo echo "1208" > /etc/qtmpv/token.txt
	
	curl https://github.com/QualiMock/qtmpv/releases/download/v0.0.2/qtmpv
	chmod +x qtmpv

	sudo mv qtmpv /opt/

	echo -e "\e[4m\e[1mqtmpv\e[0m установлен в \e[0m\e[32m/opt\e[0m\e[1m. Для запуска пропишите \e[32m/opt/qtmpv\e[0m"
}


main

