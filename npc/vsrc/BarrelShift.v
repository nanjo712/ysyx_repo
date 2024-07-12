module BarrelShift(
    input [7:0] din,
    input [2:0] shift,
    input dir,   // 0: left, 1: right
    input arith, // 0: logic, 1: arithmetic
    output [7:0] dout
);
    wire [7:0] left_1 = { din[6:0], 1'b0 };
    wire [7:0] right_1 = { arith ? din[7] : 1'b0, din[7:1] };
    wire [7:0] out1;
    wire [7:0] left_2 = { din[5:0], 2'b00};
    wire [7:0] right_2 = { arith ? 2{din[7]} : 2'b00, din[7:2] };
    wire [7:0] out2;
    wire [7:0] left_4 = { din[3:0], 4'b0000};
    wire [7:0] right_4 = { arith ? 4{din[7]} : 4'b0000, din[7:4] };
    wire [7:0] out4;
    assign dout = out4;

    genvar i;
    generate 
        for (i = 0; i < 8; i = i + 1) begin : gen
            MuxKey #(4, 2, 1) i0 (
                .out(out1[i]),
                .key({dir,shift[0]}),
                .lut({
                    2'b00, din[i],
                    2'b01, left_1[i],
                    2'b10, din[i],
                    2'b11, right_1[i]
                })
            );
        end
    endgenerate

    genvar i;
    generate 
        for (i = 0; i < 8; i = i + 1) begin : gen
            MuxKey #(4, 2, 1) i0 (
                .out(out2[i]),
                .key({dir,shift[1]}),
                .lut({
                    2'b00, out1[i],
                    2'b01, left_2[i],
                    2'b10, out1[i],
                    2'b11, right_2[i]
                })
            );
        end
    endgenerate

    genvar i;
    generate 
        for (i = 0; i < 8; i = i + 1) begin : gen
            MuxKey #(4, 2, 1) i0 (
                .out(out4[i]),
                .key({dir,shift[2]}),
                .lut({
                    2'b00, out2[i],
                    2'b01, left_4[i],
                    2'b10, out2[i],
                    2'b11, right_4[i]
                })
            );
        end
    endgenerate
    
endmodule //BarrelShift
