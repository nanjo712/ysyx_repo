module LSFR(
    input [7:0] init,
    input clk,
    input rst,
    output [7:0] out
);
    wire [2:0] shiftOp;
    shift ins(
        .in(^{out[4:2],out[0]}),
        .clk(clk),
        .shiftOp(shiftOp),
        .shiftIn(init),
        .out(out)
    );
    assign shiftOp = rst ? 3'b001 : 3'b101;
endmodule
