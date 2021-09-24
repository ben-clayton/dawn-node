#!/usr/bin/env node

// require('./build/dawnnode.node');
//require('../dawn-node/build/Debug/dawnnode.node')
require('../dawn-node/build/RelWithDebInfo/dawnnode.node')
require('./src/common/tools/setup-ts-in-node.js');
require('./src/common/runtime/cmdline.ts');

