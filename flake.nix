{
  description = "A flake to run Julia for 'Algebra Linear Aplicada'";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: {

    devShells.x86_64-linux = let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
      inself = self.devShells.x86_64-linux;
    in {
      default = inself.julia;

      julia = pkgs.mkShell rec {
        name = "Julia-shell";

        buildInputs = with pkgs; [
          julia
        ];

        shellHook = ''
          export PS1="\n\[\033[1;32m\][${name}: \w]\n\$\[\033[0m\] "
          export PURE="$([[ $IN_NIX_SHELL = 'pure' ]] && echo 1 || echo 0)"
          echo "PURE=$PURE"
          echo -n '>> Welcome to ${name}! <<'
        '';
      };
    };

  };
}
