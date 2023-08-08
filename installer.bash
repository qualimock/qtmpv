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

	sudo mv qtmpv /usr/bin/
}


main

