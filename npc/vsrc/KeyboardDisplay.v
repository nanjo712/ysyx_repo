module KeyboardDisplay(
    input clk,
    input rst,
    input ps2_clk,
    input ps2_data,
    output overflow,
    output [6:0] seg7_1,
    output [6:0] seg7_2,
    output [6:0] seg7_3,
    output [6:0] seg7_4,
    output [6:0] seg7_5,
    output [6:0] seg7_6
);
    wire [7:0] data;
    wire ready;
    reg nextdata_n;
    PS2Keyboard ins(
        .clk(clk),
        .clrn(~rst),
        .ps2_clk(ps2_clk),
        .ps2_data(ps2_data),
        .data(data),
        .ready(ready),
        .nextdata_n(nextdata_n),
        .overflow(overflow)
    );

    reg [2:0] seg7_en;
    reg [3:0] seg7_1_in;
    reg [3:0] seg7_2_in;
    reg [3:0] seg7_3_in;
    reg [3:0] seg7_4_in;
    reg is_break;
    reg [7:0] press_count;

    // SEG7 for Scan Code
    Seg7Decoder seg7_1_ins(
        .in(seg7_1_in),
        .en(seg7_en[0]),
        .out(seg7_1)
    );
    Seg7Decoder seg7_2_ins(
        .in(seg7_2_in),
        .en(seg7_en[0]),
        .out(seg7_2)
    );

    // SEG7 for ASCII Code
    Seg7Decoder seg7_3_ins(
        .in(seg7_3_in),
        .en(seg7_en[1]),
        .out(seg7_3)
    );
    Seg7Decoder seg7_4_ins(
        .in(seg7_4_in),
        .en(seg7_en[1]),
        .out(seg7_4)
    );

    ScanCodeToASCII table_ins(
        .scan_code({seg7_2_in, seg7_1_in}),
        .ascii_code({seg7_4_in, seg7_3_in})
    );

    // SEG7 for Press Count
    Seg7Decoder seg7_5_ins(
        .in(press_count[3:0]),
        .en(seg7_en[2]),
        .out(seg7_5)
    );
    Seg7Decoder seg7_6_ins(
        .in(press_count[7:4]),
        .en(seg7_en[2]),
        .out(seg7_6)
    );

    always @(posedge clk) begin
        if (rst) begin
            seg7_en <= 3'b000;
            seg7_1_in <= 4'b0000;
            seg7_2_in <= 4'b0000;
            nextdata_n <= 1;
            press_count <= 0;
        end 
        else 
        begin
            if (nextdata_n == 0) begin
                nextdata_n <= 1;
            end else
            if (ready) begin
                if (data == 8'hF0) begin
                    seg7_en <= 3'b100;
                    seg7_1_in <= 4'b0000;
                    seg7_2_in <= 4'b0000;
                    is_break <= 1;
                end
                else if (is_break) begin
                    is_break <= 0;
                end
                else begin
                    if (data != {seg7_2_in, seg7_1_in}) begin
                        press_count <= press_count + 1;
                    end
                    seg7_en <= 3'b111;
                    seg7_1_in <= data[3:0];
                    seg7_2_in <= data[7:4];

                end
                nextdata_n <= 0;
            end
        end
    end
    
endmodule //KeyboardDisplay
