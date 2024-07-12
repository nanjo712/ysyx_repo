module CodeTop(
    input [7:0] in,
    input en,
    output [2:0] out,
    output tag,
    output [6:0] seg
);
    Encoder8to3 ins1(in,en,out,tag);
    Seg7Decoder ins2({1'b0, out},en,seg);
endmodule // CodeTop
