project(
    'syros',
    'c',
    version: '0.1',
    default_options: [
        'c_std=gnu11',
        'optimization=2',
        'debug=true',
        'warning_level=2',
        'b_staticpic=false',
        'b_asneeded=false',
    ],
)

# --- Compiler Setup ---
cc = meson.get_compiler('c')

add_project_arguments(
    '-D__KERNEL__',
    '-ffreestanding',
    '-fno-pic',
    '-fno-stack-protector',
    '-fno-stack-check',
    '-m64',
    '-march=x86-64',
    '-mabi=sysv',
    '-mno-80387',
    '-mno-mmx',
    '-mno-sse',
    '-mno-sse2',
    '-mno-red-zone',
    '-mcmodel=kernel',
    '-ffunction-sections',
    '-fdata-sections',
    language: 'c',
)

# --- Linker Setup ---
add_project_link_arguments(
    '-nostdlib',
    '-static',
    '-Wl,-z,max-page-size=0x1000',
    '-Wl,--gc-sections',
    '-T' + meson.current_source_dir() / 'sys/linker.lds',
    language: 'c',
)

# --- Source Discovery ---
subdir('sys')

# --- Include Directories ---
include_dirs = ['sys', 'include']

# --- Dependencies from lib ---
subdir('lib')

# --- Executable Definition ---
executable(
    'syros',
    sources: syros_sources,
    include_directories: include_directories(include_dirs),
    dependencies: deps,
    install: true,
    name_suffix: 'elf',
)

