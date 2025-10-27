# Copyright (c) 2025 The Cicada Project. All rights reserved.
#
# THE WORK IS PROVIDED “AS IS”, WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
# WITH THE WORK OR ITS USE.

{
    description = "Cicada - dev shell";

    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
        flake-utils.url = "github:numtide/flake-utils";
    };

    outputs = { self, nixpkgs, flake-utils, ... }:
        flake-utils.lib.eachDefaultSystem (system:
            let
                pkgs = import nixpkgs { inherit system; };
            in {
                devShells.default = pkgs.mkShell {
                    name = "cicada-devshell";

                    buildInputs = with pkgs; [
                        git
                        make
                        cmake
                        gcc
                        pkg-config
                        gforth
                        python39
                    ];

                    shellHook = ''
                        export LC_ALL=C.UTF-8
                        export LANG=C.UTF-8
                        echo "Entering cicada dev shell for ${system}"
                    '';
                };

                # optional simple placeholder package so `nix build` has something to build
                packages.default = pkgs.stdenv.mkDerivation {
                    name = "cicada-build";
                    buildCommand = ''
                        make re
                    '';
                };
            });
}