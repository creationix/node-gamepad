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
        ['OS == "mac"', {
          'sources': [ '<(gamepad_dir)/Gamepad_macosx.c' ],
          'LDFLAGS': [
            '-framework IOKit',
            '-framework CoreFoundation'
          ],
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'OTHER_LDFLAGS': [
              '-framework IOKit',
              '-framework CoreFoundation'
            ],
          }
        }],
        ['OS == "win"', { 'sources': [
          '<(gamepad_dir)/Gamepad_windows_dinput.c'
        ]}]
      ],
      'include_dirs': [
        './gamepad/source',
        "<!(node -e \"require('nan')\")"
      ],
    }
  ]
}
