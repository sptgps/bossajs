const EventEmitter = require('events').EventEmitter;
const promisify = require('util').promisify;

const addon = require('./build/Release/bossa.node');

// Base C++ addon on EventEmitter
addon.Bossa.prototype.__proto__ = EventEmitter.prototype;

// Wrap C++ addon to use promises
class Bossa extends addon.Bossa {
    constructor(...args) {
        super(...args);

        for (const method of [
            'connect',
            'close',
            'erase',
            'info',
            'read',
            'write',
            'verify',
        ]) {
            this[method] = promisify(this[method].bind(this));
        }
    }
}

module.exports = {
    Bossa,
}