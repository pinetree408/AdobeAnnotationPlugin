# AdobeAnnotationPlugin
> Annotation save & load plugin for Adobe

## Dependency
- Adobe DC (Reader, Pro)
- Visual Studio 2013

## Source
- source/
- config.js

## Security Issue
If plug-in accesses fdf file(annotations) directly, It can make security issue at Adobe DC client.
Therefore, it must be turn off advanced security settings and add fdf file storage path to safe checking setting.

## How to set up
1. compile plugin source
2. copy compiled soucre at 'C:\Program Files (x86)\Adobe\Acrobat DC\Acrobat\plug_ins'
3. copy config.js javascript file at 'C:\Program Files (x86)\Adobe\Acrobat DC\Acrobat\Javascripts'
