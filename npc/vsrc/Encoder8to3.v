module  Encoder8to3(
    input [7:0] in,
    input en,
    output reg [2:0] out,
    output tag
);
    assign tag = |in;
    // assign out = ~en   ? 3'b000 : 
    //              in[7] ? 3'b111 : 
    //              in[6] ? 3'b110 : 
    //              in[5] ? 3'b101 : 
    //              in[4] ? 3'b100 : 
    //              in[3] ? 3'b011 : 
    //              in[2] ? 3'b010 : 
    //              in[1] ? 3'b001 : 
    //              3'b000;
    
    always @(*) begin
        if (en)
            casez (in)
                8'b00000001: out = 3'b000;
                8'b0000001z: out = 3'b001;
                8'b000001zz: out = 3'b010;
                8'b00001zzz: out = 3'b011;
                8'b0001zzzz: out = 3'b100;
                8'b001zzzzz: out = 3'b101;
                8'b01zzzzzz: out = 3'b110;
                8'b1zzzzzzz: out = 3'b111; 
                default: out = 3'b000;  
            endcase
        else out = 3'b000;
    end
    
endmodule // Encoder8to3
