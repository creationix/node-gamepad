{
  'variables': {
      'gamepad_dir%': 'gamepad/source/gamepad'
  },
  'targets': [
    {
      'target_name': 'gamepad',
      'sources': [
        'binding.cc',
        '<(gamepad_dir)/Gamepad_private.c',
      ],
      'conditions': [
        ['OS == "linux"', { 'sources': [ '<(gamepad_dir)/Gamepad_linux.c' ]}],
        ['OS == "mac"', { 'sources': [ '<(gamepad_dir)/Gamepad_macosx.c' ]}],
        ['OS == "windows"', { 'sources': [
          '<(gamepad_dir)/Gamepad_windows_dinput.c'
          '<(gamepad_dir)/Gamepad_windows_mm.c'
        ]}]
      ],
      'include_dirs': [
        './gamepad/source',
      ],
    }
  ]
}
