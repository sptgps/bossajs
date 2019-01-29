const { promisify } = require('util');

const { assert, expect } = require('chai');


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
            await promisify(bossa.info.bind(bossa))();
            assert.fail("Not reached");
        } catch (e) {
            expect(e).is.a('Error');
        }
    });

    it("info", async () => {
        const bossa = new Bossa();

        await promisify(bossa.connect.bind(bossa))(process.env.PORT);
        const info = await promisify(bossa.info.bind(bossa))();

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

    it("read", async () => {
        const bossa = new Bossa();
        await promisify(bossa.connect.bind(bossa))(process.env.PORT);

        const data = await promisify(bossa.read.bind(bossa))(0x0, 512);
        expect(data).to.be.an.instanceOf(Buffer);
        expect(data).to.have.length(512);
    });
});