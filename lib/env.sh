#!/usr/bin/env bash

system=""
uname -v | grep "Darwin" >/dev/null 2>&1
if [[ $? == 0 ]]; then system="Darwin"; fi
uname -v | grep "Debian" >/dev/null 2>&1
if [[ $? == 0 ]]; then system="Debian"; fi

export system
export basedir=`dirname ${BASH_SOURCE[0]}`"/.."
basedir=`$basedir/bin/realpath $basedir`

if [[ -n $0 && "$0" != "-bash" ]]; then
	script=`basename $0`
else
	script=""
fi

alias gd='git diff | colordiff | less -R'

if [[ `hostname` == "sranawebln01" ]]; then
	export cfg_file="config-sranawebln01.py"
else
	export cfg_file="config-dev.py"
fi

. $basedir/etc/$cfg_file

PS1="<\[\033[31m\]\h\[\033[0m\]:\[\033[36m\]$server\[\033[0m\]> \[\033[34m\]\W\[\033[0m\]\[\033[37m\]%\[\033[0m\] "
