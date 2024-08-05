module ALU(
    input [3:0] A,
    input [3:0] B,
    input [2:0] ALUop,
    output [3:0] out,
    output zero,
    output carry,
    output overflow
);
    wire flag = ~(ALUop[2] | ALUop[1]) ? ALUop[0] : 
                 (ALUop[2] & ALUop[1]) ? 1 : 0;

    wire [3:0] adder_out;
    adder ins(
        .A(A),
        .B(B),
        .flag(flag),
        .out(adder_out),
        .cout(carry),
        .overflow(overflow),
        .zero(zero)
    );

    assign out = ALUop[2:1] == 2'b00 ? adder_out :
                 ALUop[2:0] == 3'b010 ? ~A :
                 ALUop[2:0] == 3'b011 ? A & B :
                 ALUop[2:0] == 3'b100 ? A | B :
                 ALUop[2:0] == 3'b101 ? A ^ B : 
                 ALUop[2:0] == 3'b110 ? {3'b000, overflow ^ adder_out[3]} :
                 ALUop[2:0] == 3'b111 ? {3'b000, zero} : 4'b0000;

endmodule //ALU

