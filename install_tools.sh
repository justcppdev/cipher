#!/bin/bash
set -e

if [ `uname` != "Darwin" ]
then
    echo "It only supports the macOS system."
    exit -1
fi

is_installed() {
    [ `which "$1"` ]
}

if ! is_installed brew
then
    ruby -e "$(curl -fsSL \
        https://raw.githubusercontent.com/Homebrew/install/master/install)"
fi

brew upgrade
brew install cmake git python3
if ! is_installed pip3
then sudo easy_install pip
fi

pip3 install --upgrade \
    pip setuptools wheel tox

git submodule update --init --recursive --remote
