{
  description = "A flake to run Julia for 'Algebra Linear Aplicada'";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        myShell = input@{
            name,
            buildInputs ? [],
            toShellName ? n: "${n}-shell",
            shellHook ? { shellName, ... }: ''
              export PS1="\n\[\033[1;32m\][${shellName}: \w]\n\$\[\033[0m\] "
              export PURE="$([[ $IN_NIX_SHELL = 'pure' ]] && echo 1 || echo 0)"
              echo "PURE=$PURE"
              echo -n '>> Welcome to ${shellName}! <<'
            '',
          }: pkgs.mkShell {
            name = toShellName name;

            buildInputs = buildInputs;

            shellHook = shellHook
              (input // { shellName = toShellName name; });
          };
      in {
        devShells = {
            default = self.devShells.${system}.julia;

            julia = myShell {
              name = "Julia";
              buildInputs = with pkgs; [ julia ];
            };

            j = myShell {
              name = "j";
              buildInputs = with pkgs; [ j ];
            };

            slides = myShell {
              name = "slides";
              buildInputs = with pkgs; [ slides ];
            };

            latex = myShell {
              name = "latex";
              buildInputs = with pkgs; [
                texlive.combined.scheme-small
                zathura
              ];
            };

            calc = myShell {
              name = "stream-calc";
              buildInputs = with pkgs; [ rlwrap ];
            };

            dot = myShell {
              name = "dot";
              buildInputs = with pkgs; [ graphviz ];
            };
          };
      });
}
