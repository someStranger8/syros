# --- Flanterm Library ---
flanterm_sources = [
    'flanterm/src/flanterm.c',
    'flanterm/src/flanterm_backends/fb.c',
]

flanterm_includes = include_directories('flanterm/src/')

flanterm_lib = static_library(
    'flanterm',
    flanterm_sources,
    include_directories: flanterm_includes,
    pic: false,
)

flanterm_dep = declare_dependency(
    include_directories: flanterm_includes,
    link_with: flanterm_lib,
)

# --- Dependencies ---
deps = [flanterm_dep]
