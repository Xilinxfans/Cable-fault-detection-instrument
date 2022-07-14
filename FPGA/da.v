module da_wave_send(
    input                 sys_clk    ,  //时钟
    input                 sys_rst_n  ,  //复位信号，低电平有效
	input                 sys_rst_n1  ,  //复位信号，低电平有效
    input        [7:0]    rd_data,  	 //ROM_1M读出的数据 
	input        [7:0]    rd_data1,  	 //ROM_5M读出的数据
	 
    output  reg  [9:0]    rd_addr,  //读ROM_1M地址
	output  reg  [9:0]    rd_addr1,  //读ROM_5M地址
    output                da_clk ,  //DA(AD9708)驱动时钟,最大支持125Mhz时钟
    output       [7:0]    da_data   //输出给DA的数据  
    );


//*****************************************************
//**                    main code
//*****************************************************

//数据rd_data是在clk的上升沿更新的，所以DA芯片在clk的下降沿锁存数据是稳定的时刻
//而DA实际上在da_clk的上升沿锁存数据,所以时钟取反,这样clk的下降沿相当于da_clk的上升沿
assign  da_clk = ~sys_clk;       
assign  da_data = sys_rst_n ? rd_data : rd_data1;   //将读到的ROM数据赋值给DA数据端口

//读ROM_1M地址  
always @(posedge sys_clk or negedge sys_rst_n) begin
    if(sys_rst_n == 1'b0)
        rd_addr <= 10'b0;
	 else 
        rd_addr <= rd_addr + 10'b1;
end
//读ROM_5M地址
always @(posedge sys_clk or negedge sys_rst_n1) begin
    if(sys_rst_n1 == 1'b0)
        rd_addr1 <= 10'b0;
	 else 
        rd_addr1 <= rd_addr1 + 10'b1;
end

endmodule
