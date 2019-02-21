[![Build Status](https://travis-ci.org/rock-core/base-numeric.svg?branch=master)](https:///travis-ci.org/rock-core/base-numeric)

# Numeric

This library is a loose collection of tools which help with problems in
statistics, data analysis and the like

## Installation
Create a new Rock-based installation in a development folder, here called dev:

```
  mkdir dev
  cd dev
  wget http://www.rock-robotics.org/master/autoproj_bootstrap
  ruby autoproj_bootstrap
```

In autoproj/manifest add the respective manifest and add the package to the layout section:

```
package_set:
    - github: rock-core/package_set

layout:
    - base/numeric
```

```
$>source env.sh
$>autoproj update
$>autoproj osdeps
$>amake base/numeric
```

### Docker-based
If you want to use only a docker-based image, then:
Install docker.io and first build the image with the provided Dockerfile

```
    git clone https://github.com/rock-core/base-numeric.git .
    cd base-numeric
    docker build -t rock/base-numeric --build-arg PKG_NAME="base/numeric" .
```

To login into the built image:
```
    docker run -it rock/base-numeric /bin/bash
```

## License

This work is licensed under [LGPL v2 or later](https://opensource.org/licenses/LGPL-2.1)

## Copyright
Copyright (c) Jakob Schwendner and Contributors, DFKI GmbH Robotics Innovation
Center, Brazilian Institute of Robotics (BIR)

