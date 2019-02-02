# Bossa.js

Node bindings for [Bossa](https://github.com/shumatech/BOSSA), the open source
Atmel SAM-BA DFU utility.

Currently only working on macOS. Other platforms coming soon.

## Installing

```
yarn add bossajs
```

Or from source:

```
git submodule update --init
yarn rebuild
```

### Windows

Only `win32` is supported.

As a superuser install Python and MSVS build tools:

```
npm install --global --production windows-build-tools
```

Then build the app as:

```
npm config set arch ia32 
git submodule update --init
node-gyp configure --arch x86
node-gyp rebuild
```

## Usage

```js
import { Bossa } from 'bossaja';

const PORT = '/dev/tty.usb1';
const bossa = new Bossa();

await bossa.connect(PORT);

try {
    const buffer = Buffer.from([0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf]);
    await bossa.write(buffer, 0x2000);
    await bossa.verify(buffer, 0x2000);
} finally {
    await bossa.close();
}
```

## API Documentation

### Constructor

* `Bossa()` / `Bossa(opts)`

  `opts` is an optional object of options.

  ```js
  {
      debug: boolean,
  }
  ```

### Methods

* `connect(port)` -> `Promise`

  Connect to the device.

* `close()` -> `Promise`

  Close the connection.

* `info()` -> `Promise<Object>`

  Return info about the device.

* `read(offset, size)` -> `Promise<Buffer>`

  Read `size` bytes starting from `offset`.

* `write(buffer, offset)` -> `Promise`

  Write `buffer` to device starting from `offset`.

* `verify(buffer, offset)` -> `Promise`

  Verify the data at `offset` is equivalent to `buffer`.

## Events

Inherits from [`EventEmitter`](https://nodejs.org/api/events.html).

* `progress` -> `(progress, total)`

  Progress update of the current operation (in device pages).

## Testing

Testing is possible on a development board running the SAM-BA bootloader,
e.g. the Atmel SAMD21 Xplained Pro.

```
PORT=/dev/tty.usb1 yarn test
```

## Copyright and License

**Bossa.js** is &copy; 2019, Sports Performance Tracking. It is released under
a BSD 3-clause license, which you can find in `LICENSE`.

**Bossa** is &copy; 2011-2016, ShumaTech. It is also released under a BSD
3-clause license, which you can find in `extern/bossa/LICENSE`.