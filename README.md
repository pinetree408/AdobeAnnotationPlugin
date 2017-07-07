# AdobeAnnotationPlugin
## Dependency
- Adobe DC (Reader, Pro)
- Visual Studio 2013

## Source
- source/
- config.js

## Security Issue
If plug-in accesses fdf file(annotations) directly, It can make security issue at Adobe DC client.
Therefore, it must be turn off advanced security settings and add fdf file storage path to safe checking setting.
