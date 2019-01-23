var module;

if (process.env.DEBUG) {
    module = require('./build/Debug/bossa.node');
} else {
    module = require('./build/Release/bossa.node')
}

module.exports = module;