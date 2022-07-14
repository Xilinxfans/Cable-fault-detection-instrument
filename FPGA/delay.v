module delay(
    input			  sys_clk,                  //系统时钟
    input             sys_rst_n,                //系统复位，低电平有效
    input   [7:0]	  uart_da,                  //接收的数据
    
    output  reg       uart_en,					//	1M_da采集使能
	output  reg       uart_en3,				    //	5M_da采集使能
	output  reg       uart_en1,				    // uart_send采集数据使能
	output  reg       uart_en2					// ad发送使能
);
//reg define
reg  [7:0]	uart_in_data;	
reg  [21:0]	uart_cnt;

always @(posedge sys_clk or negedge sys_rst_n) 
    if (!sys_rst_n)
		uart_in_data <= 8'd0;
	 else
		uart_in_data <= uart_da;
	 
always @(posedge sys_clk or negedge sys_rst_n) begin 
    if (!sys_rst_n) 
		uart_cnt <= 22'd0;
	 else if((uart_in_data == 8'd255) || (uart_in_data == 8'd239)) begin
			uart_cnt <= uart_cnt + 1'b1;
				if(uart_cnt == 22'd2250500)
					uart_cnt <= uart_cnt;
		end
		else 
			uart_cnt <= 22'd0;
end
		
always @(posedge sys_clk or negedge sys_rst_n) begin 
    if (!sys_rst_n) begin
		uart_en <= 1'b0;
		uart_en1 <= 1'b0;
		uart_en2 <= 1'b0;
	 end
    else if(uart_in_data == 8'd255) begin
				uart_en <= 1'b1;
				uart_en2 <= 1'b1;
			if(uart_cnt >= 22'd500)
				uart_en <= 1'b0;
			if(uart_cnt >= 22'd900)
				uart_en2 <= 1'b0;
				uart_en1 <= 1'b1;
			if(uart_cnt == 22'd2250500)	
				uart_en1 <= 1'b0;	
			end
	 else	if(uart_in_data == 8'd239) begin
				uart_en3 <= 1'b1;
				uart_en2 <= 1'b1;
			if(uart_cnt >= 22'd500)
				uart_en3 <= 1'b0;
			if(uart_cnt >= 22'd900)
				uart_en2 <= 1'b0;
				uart_en1 <= 1'b1;
			if(uart_cnt == 22'd2250500)	
				uart_en1 <= 1'b0;	
			end
	 else begin
				uart_en <= 1'b0;
				uart_en1 <= 1'b0;
				uart_en2 <= 1'b0;
				uart_en3 <= 1'b0;
			end
end
endmodule	 		 
