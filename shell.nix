# Copyright (c) 2025 The Cicada Project. All rights reserved.
#
# THE WORK IS PROVIDED “AS IS”, WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
# WITH THE WORK OR ITS USE.

{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
    # Add the packages you need for development here
    buildInputs = with pkgs; [
        git
        gcc
        make
        python39
        gforth
        vim
    ];

    nativeBuildInputs = with pkgs; [
        pkgconfig
    ];

    shellHook = ''
        echo "Welcome to the dev shell for cicada"
        export EDITOR=vim
    '';
}