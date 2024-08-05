module shift(
    input in,
    input clk,
    input [2:0] shiftOp,
    input [7:0] shiftIn,
    output reg [7:0] out
);  
    wire d1, d2, d3, d4, d5, d6, d7, d8;
    wire q1, q2, q3, q4, q5, q6, q7, q8;
    assign out = {q8, q7, q6, q5, q4, q3, q2, q1};
    assign d1 = shiftOp == 3'b000 ? 0 : // RESET
                shiftOp == 3'b001 ? shiftIn[0] : // PARALLEL IN
                shiftOp == 3'b010 ? q2 : // LOGIC SHIFT RIGHT
                shiftOp == 3'b011 ? 0 :  // LOGIC SHIFT LEFT
                shiftOp == 3'b100 ? q2 : // ARITH SHIFT RIGHT
                shiftOp == 3'b101 ? q2 : // SERIAL IN
                shiftOp == 3'b110 ? q2 : // ROTATE RIGHT
                shiftOp == 3'b111 ? q8 : 0; // ROTATE LEFT

    assign d2 = shiftOp == 3'b000 ? 0 : // RESET
                shiftOp == 3'b001 ? shiftIn[1] : // PARALLEL IN
                shiftOp == 3'b010 ? q3 : // LOGIC SHIFT RIGHT
                shiftOp == 3'b011 ? q1 :  // LOGIC SHIFT LEFT
                shiftOp == 3'b100 ? q3 : // ARITH SHIFT RIGHT
                shiftOp == 3'b101 ? q3 : // SERIAL IN
                shiftOp == 3'b110 ? q3 : // ROTATE RIGHT
                shiftOp == 3'b111 ? q1 : 0; // ROTATE LEFT
    
    assign d3 = shiftOp == 3'b000 ? 0 : // RESET
                shiftOp == 3'b001 ? shiftIn[2] : // PARALLEL IN
                shiftOp == 3'b010 ? q4 : // LOGIC SHIFT RIGHT
                shiftOp == 3'b011 ? q2 :  // LOGIC SHIFT LEFT
                shiftOp == 3'b100 ? q4 : // ARITH SHIFT RIGHT
                shiftOp == 3'b101 ? q4 : // SERIAL IN
                shiftOp == 3'b110 ? q4 : // ROTATE RIGHT
                shiftOp == 3'b111 ? q2 : 0; // ROTATE LEFT

    assign d4 = shiftOp == 3'b000 ? 0 : // RESET
                shiftOp == 3'b001 ? shiftIn[3] : // PARALLEL IN
                shiftOp == 3'b010 ? q5 : // LOGIC SHIFT RIGHT
                shiftOp == 3'b011 ? q3 :  // LOGIC SHIFT LEFT
                shiftOp == 3'b100 ? q5 : // ARITH SHIFT RIGHT
                shiftOp == 3'b101 ? q5 : // SERIAL IN
                shiftOp == 3'b110 ? q5 : // ROTATE RIGHT
                shiftOp == 3'b111 ? q3 : 0; // ROTATE LEFT
    
    assign d5 = shiftOp == 3'b000 ? 0 : // RESET
                shiftOp == 3'b001 ? shiftIn[4] : // PARALLEL IN
                shiftOp == 3'b010 ? q6 : // LOGIC SHIFT RIGHT
                shiftOp == 3'b011 ? q4 :  // LOGIC SHIFT LEFT
                shiftOp == 3'b100 ? q6 : // ARITH SHIFT RIGHT
                shiftOp == 3'b101 ? q6 : // SERIAL IN
                shiftOp == 3'b110 ? q6 : // ROTATE RIGHT
                shiftOp == 3'b111 ? q4 : 0; // ROTATE LEFT

    assign d6 = shiftOp == 3'b000 ? 0 : // RESET
                shiftOp == 3'b001 ? shiftIn[5] : // PARALLEL IN
                shiftOp == 3'b010 ? q7 : // LOGIC SHIFT RIGHT
                shiftOp == 3'b011 ? q5 :  // LOGIC SHIFT LEFT
                shiftOp == 3'b100 ? q7 : // ARITH SHIFT RIGHT
                shiftOp == 3'b101 ? q7 : // SERIAL IN
                shiftOp == 3'b110 ? q7 : // ROTATE RIGHT
                shiftOp == 3'b111 ? q5 : 0; // ROTATE LEFT

    assign d7 = shiftOp == 3'b000 ? 0 : // RESET
                shiftOp == 3'b001 ? shiftIn[6] : // PARALLEL IN
                shiftOp == 3'b010 ? q8 : // LOGIC SHIFT RIGHT
                shiftOp == 3'b011 ? q6 :  // LOGIC SHIFT LEFT
                shiftOp == 3'b100 ? q8 : // ARITH SHIFT RIGHT
                shiftOp == 3'b101 ? q8 : // SERIAL IN
                shiftOp == 3'b110 ? q8 : // ROTATE RIGHT
                shiftOp == 3'b111 ? q6 : 0; // ROTATE LEFT

    assign d8 = shiftOp == 3'b000 ? 0 : // RESET
                shiftOp == 3'b001 ? shiftIn[7] : // PARALLEL IN
                shiftOp == 3'b010 ? 0 : // LOGIC SHIFT RIGHT
                shiftOp == 3'b011 ? q7 :  // LOGIC SHIFT LEFT
                shiftOp == 3'b100 ? q8 : // ARITH SHIFT RIGHT
                shiftOp == 3'b101 ? in : // SERIAL IN
                shiftOp == 3'b110 ? q1 : // ROTATE RIGHT
                shiftOp == 3'b111 ? q7 : 0; // ROTATE LEFT
    
    dff dff1(
        .clk(clk),
        .d(d1),
        .q(q1)
    );
    dff dff2(
        .clk(clk),
        .d(d2),
        .q(q2)
    );
    dff dff3(
        .clk(clk),
        .d(d3),
        .q(q3)
    );
    dff dff4(
        .clk(clk),
        .d(d4),
        .q(q4)
    );
    dff dff5(
        .clk(clk),
        .d(d5),
        .q(q5)
    );
    dff dff6(
        .clk(clk),
        .d(d6),
        .q(q6)
    );
    dff dff7(
        .clk(clk),
        .d(d7),
        .q(q7)
    );
    dff dff8(
        .clk(clk),
        .d(d8),
        .q(q8)
    );
endmodule //shift

