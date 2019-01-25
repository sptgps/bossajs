const { expect } = require('chai');


describe("Bossa", () => {
    const Bossa = require('../').default;

    it("requires new", () => {
        expect(() => {
            Bossa();
        }).to.throw(Error, "Must call with new keyword");
    });

    it("requires an object", () => {
        expect(() => {
            new Bossa();
        }).to.throw(TypeError);

        expect(() => {
            new Bossa(1);
        }).to.throw(TypeError);

        expect(new Bossa({
            port: process.env.PORT,
        })).to.be.a('Bossa');
    });

    it("info", done => {
        const bossa = new Bossa({
            port: process.env.PORT,
            debug: true,
        });
        const info = bossa.info((err, info) => {
            expect(err).to.be.null;
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

            done();
        });
    });
});