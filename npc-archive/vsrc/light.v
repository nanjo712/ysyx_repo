module light(
    input clk,
    input rst,
    output reg [15:0] led
);
    reg[31:0] count;
    always @(posedge clk) begin
        if (rst) begin
            count <= 0;
            led <= 1;
        end 
        else begin
            count <= count + 1;
            if (count == 5000000) begin
                led <= {led[14:0], led[15]};
                count <= 0;
            end
        end
    end
endmodule
