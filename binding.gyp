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
        ['OS == "win"', { 'sources': [
          '<(gamepad_dir)/Gamepad_windows_dinput.c'
        ]}]
      ],
      'include_dirs': [
        './gamepad/source',
      ],
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
