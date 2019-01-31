const { assert, expect } = require('chai');


describe("Bossa", () => {
    const Bossa = require('../').default;

    it("requires new", () => {
        expect(() => {
            Bossa();
        }).to.throw(Error, "Bossa cannot be invoked without 'new'");
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
        bossa.connect(process.env.PORT).then(done);
    });

    it("must be connected", async () => {
        const bossa = new Bossa();
    
        try {
            await bossa.info();
            assert.fail("Not reached");
        } catch (e) {
            expect(e).is.a('Error');
        }
    });

    it("info", async () => {
        const bossa = new Bossa();

        await bossa.connect(process.env.PORT);
        const info = await bossa.info();

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
        await bossa.connect(process.env.PORT);

        const data = await bossa.read(0x0, 512);
        expect(data).to.be.an.instanceOf(Buffer);
        expect(data).to.have.length(512);
    });

    it("erase", async function () {
        this.skip("Slow");
        this.timeout(20000);

        const bossa = new Bossa();
        await bossa.connect(process.env.PORT);
        await bossa.erase(0x2000);
    });

    it("write and read", async () => {
        const bossa = new Bossa();
        const buffer = Buffer.from([0xd, 0xe, 0xa, 0xd, 0x0, 0xb, 0xe, 0xe, 0xf]);

        await bossa.connect(process.env.PORT);

        try {
            await bossa.write(buffer, 0x3000);
            
            const result = await bossa.read(0x3000, buffer.length);
            expect(result).to.deep.equal(buffer);
        } finally {
            await bossa.close();
        }
    });

    describe("write and verify", () => {
        it("good", async () => {
            const bossa = new Bossa();
            const buffer = Buffer.from([0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf]);
    
            await bossa.connect(process.env.PORT);

            try {
                await bossa.write(buffer, 0x3000);
                await bossa.verify(buffer, 0x3000);
            } finally {
                await bossa.close();
            }
        });

        it("bad", async () => {
            const bossa = new Bossa();
            let buffer;

            await bossa.connect(process.env.PORT);

            try {
                buffer = Buffer.from([0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf]);
                await bossa.write(buffer, 0x3000);
                
                buffer = Buffer.from([0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xd]);
                
                try {
                    await bossa.verify(buffer, 0x3000);
                    assert.fail("Not reached");
                } catch (e) {
                    expect(e).is.a('Error');
                }

            } finally {
                await bossa.close();
            }
        });
    });

    it("on progress", async () => {
        const bossa = new Bossa();
        const events = [];

        bossa.on('progress', (...args) => {
            events.push([...args]);
        });

        await bossa.connect(process.env.PORT);
        await bossa.read(0x0, 512);

        expect(events).to.deep.equal([
            [0, 8],
            [1, 8],
            [2, 8],
            [3, 8],
            [4, 8],
            [5, 8],
            [6, 8],
            [7, 8],
            [8, 8],
        ]);
    });
});