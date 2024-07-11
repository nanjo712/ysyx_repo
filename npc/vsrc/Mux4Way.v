module Mux4Way(
    input [3:0] x,
    input [1:0] y,
    output f
);
    assign f = (~y[0]&~y[1]&x[0]) | (y[0]&~y[1]&x[1]) | (~y[0]&y[1]&x[2]) | (y[0]&y[1]&x[3]);
endmodule
