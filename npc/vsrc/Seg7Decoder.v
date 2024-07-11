module Seg7Decoder(
    input [2:0] in,
    output [6:0] out
);
    assign out = in == 3'b000 ? 7'b1000000 :
                 in == 3'b001 ? 7'b1111001 :
                 in == 3'b010 ? 7'b0100100 :
                 in == 3'b011 ? 7'b0110000 :
                 in == 3'b100 ? 7'b0011001 :
                 in == 3'b101 ? 7'b0010010 :
                 in == 3'b110 ? 7'b0000010 :
                 in == 3'b111 ? 7'b1111000 :
                 7'b1111111;  
endmodule //Seg7Decoder
