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

    it("info", () => {
        const bossa = new Bossa({
            port: process.env.PORT,
            debug: true,
            info: true,
        });
    });
});