module ad_wave_rec(
    input                  sys_rst_n    ,  //复位信号，低电平有效
    input          [7:0]   ad_data     ,   //AD输入数据
	input                  clk_30M      ,  //时钟
	 
    output                 ad_clk  ,       //AD(TLC5510)驱动时钟,最大支持20Mhz时钟
	output   reg           ram_wr_en  , 
	output   reg   [8:0]   addr_ad,
	output   wire  [7:0]   ad_data_out
    );
	 	 
//*****************************************************
//**                    main code 
//*****************************************************    
assign ad_clk = clk_30M;

always @(posedge clk_30M or negedge sys_rst_n) begin
    if(sys_rst_n == 1'b0) begin
			ram_wr_en <= 1'b0;
			addr_ad <= 9'd0;
	 end
	 else begin
			if(addr_ad < 9'd511) begin
					ram_wr_en <= 1'b1;
					addr_ad <= addr_ad + 1'b1;
			end
			else if(addr_ad == 9'd511)begin
					addr_ad <= addr_ad;
					ram_wr_en <= 1'b0;
			end
			else begin
					addr_ad <= 9'd0;
					ram_wr_en <= 1'b0;
			end
	 end
end

assign  ad_data_out = sys_rst_n ? ad_data : 8'd0;

endmodule
