module Mux4Way2Bit(
    input [1:0] x0,
    input [1:0] x1,
    input [1:0] x2,
    input [1:0] x3,
    input [1:0] y,
    output [1:0] f 
);
    Mux4Way ins1({x3[0],x2[0],x1[0],x0[0]},y,f[0]);
    Mux4Way ins2({x3[1],x2[1],x1[1],x0[1]},y,f[1]);
endmodule
