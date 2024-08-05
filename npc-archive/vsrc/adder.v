module adder(
    input [3:0] A,
    input [3:0] B,
    input flag,
    output [3:0] out,
    output cout,
    output overflow,
    output zero
);
    wire [3:0] t_B = ({4{flag}} ^ B);
    assign {cout, out} = A + t_B + + {3'b000, flag};
    assign overflow = (A[3] & t_B[3] & ~out[3]) | (~A[3] & ~t_B[3] & out[3]);
    assign zero = ~(|out);
endmodule //adder

