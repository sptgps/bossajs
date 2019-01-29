const { promisify } = require('util');

const { assert, expect } = require('chai');


function toPromise(obj, method, ...args) {
    return promisify(obj[method].bind(obj))(...args);
}


describe("Bossa", () => {
    const Bossa = require('../').default;

    it("requires new", () => {
        expect(() => {
            Bossa();
        }).to.throw(Error, "Must call with new keyword");
    });

    it("require an object or nothing", () => {
        expect(() => {
            new Bossa(1);
        }).to.throw(TypeError);

        expect(new Bossa()).to.be.a('Bossa');
        expect(new Bossa({})).to.be.a('Bossa');
    });

    it("connect", done => {
        const bossa = new Bossa();
        bossa.connect(process.env.PORT, done);
    });

    it("must be connected", async () => {
        const bossa = new Bossa();
    
        try {
            await toPromise(bossa, 'info');
            assert.fail("Not reached");
        } catch (e) {
            expect(e).is.a('Error');
        }
    });

    it("info", async () => {
        const bossa = new Bossa();

        await toPromise(bossa, 'connect', process.env.PORT);
        const info = await toPromise(bossa, 'info');

        expect(info).to.be.an('object');
        expect(info).to.have.keys(
            "name",
            "chipId",
            "extChipId",
            "version",
            "address",
            "numPages",
            "pageSize",
            "totalSize",
            "numPlanes",

            "security",
            "bootFlash",
            "bod",
            "bor",

            "canBootFlash",
            "canBod",
            "canBor",
            "canChipErase",
            "canWriteBuffer",
            "canChecksumBuffer",

            "lockRegions",
        );
    });

    it("read first 512 bytes", async () => {
        const bossa = new Bossa();
        await toPromise(bossa, 'connect', process.env.PORT);

        const data = await toPromise(bossa, 'read', 0x0, 512);
        expect(data).to.be.an.instanceOf(Buffer);
        expect(data).to.have.length(512);
    });

    it("erase", async function () {
        this.skip("Slow");
        this.timeout(20000);

        const bossa = new Bossa();
        await toPromise(bossa, 'connect', process.env.PORT);
        await toPromise(bossa, 'erase', 0x2000);
    });

    it("write and read", async () => {
        const bossa = new Bossa();
        const buffer = Buffer.from([0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf]);

        await toPromise(bossa, 'connect', process.env.PORT);
        await toPromise(bossa, 'write', buffer, 0x3000);

        const result = await toPromise(bossa, 'read', 0x3000, buffer.length);
        expect(result).to.deep.equal(buffer);
    });
});