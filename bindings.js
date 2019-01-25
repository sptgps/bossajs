let bossa;

if (process.env.DEBUG) {
    bossa = require('./build/Debug/bossa.node');
} else {
    bossa = require('./build/Release/bossa.node');
}

module.exports = bossa;