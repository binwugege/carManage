<template>
	<view class="main">
		<view class="onCarData">
			<ul>
				<li style="border-bottom: 1px solid #b9b9b9;text-align: center;">
					<text style="font-size: 18px;">请输入车牌号&nbsp;&nbsp;</text>
					<input type="text" v-model="carPlateData" placeholder="请输入车牌号" style="width:55%;height:100%;display: inline-block;vertical-align:middle;font-size: 18px;border-left: 1px solid #dedede;">
				</li>
				<li style="margin-top:10px;">
					<button type="default" size="mini" @click="selectOrder">查询订单</button>
					</li>
			</ul>
		</view>
		<view class="haveCarData" v-if="isCarPlate">
			<ul>
				<li style="border-bottom: 1px solid #e5e5e5;text-align: center;line-height: 60px;">
					<text style="font-size: 18px;margin-top: 10px;">进站时间：{{carOrderData.startTimer}}&nbsp;&nbsp;</text>
				</li>
				<li style="border-bottom: 1px solid #e5e5e5;text-align: center;line-height: 60px;">
					<text style="font-size: 18px;">出站时间：{{carOrderData.endTimer}}&nbsp;&nbsp;</text>
				</li>
				<li style="border-bottom: 1px solid #e5e5e5;text-align: center;line-height: 60px;">
					<text style="font-size: 18px;">总共停车时间：{{parkingTimer}}分钟&nbsp;&nbsp;</text>
				</li>
				<li style="text-align: center;line-height: 60px;">
					<button type="primary" size="mini" style="display: block;margin-top: 12px;font-size: 18px;" @click="payOrder">立即支付</button>
				</li>
			</ul>
		</view>
		<uni-popup ref="carOrder" type="dialog" style="width: 100%;height: 300rpx;">
			<div class="payorder" style="width: 300px;height:100px">
				<button type="primary" @click="close"
					style="width: 40%;background-color:#fbffff;color:black;display: inline-block;">取消</button>
				<button type="primary" @click="confirm"
					style="width: 40%;background-color:#409EFF;color: black;display: inline-block;">确认</button>
			</div>
		</uni-popup>
	</view>
</template>

<script>
import { formatTime } from "../../utils/util.js"
	export default {
		data() {
			return {
				carOrderData:{}, // 订单数据 
				isCarPlate:false, // 是否查询成功
				carPlateData:"", // 车牌信息
				getCarPayUrl:"http://localhost:8081/pay/getOrder",
				accountPayUrl:"http://localhost:8081/pay/accountOrder", // 结算的URL
				parkingTimer:0, // 停车时间
			}
		},
		methods: {
			payOrder(){
				this.$refs.carOrder.open();
			},
			close(){
				this.$refs.carOrder.close();
			},
			confirm(){
				// this.accountOrder();
				uni.showToast({
					title:"支付成功"
				})
				this.$refs.carOrder.close();
			},
			sendAxios(url, methos, data) {
				return new Promise((resolve, reject) => {
					uni.request({
						url: url,
						data: data,
						method: methos,
						header: {
							"content-type": "application/x-www-form-urlencoded"
						},
						success(res) {
							resolve(res.data);
						},
						fail(res) {
							reject(res.data);
						}
					})
				})
			},
			async selectOrder(){
				let data = {};
				const carPayUrl = `${this.getCarPayUrl}?carPlate=${this.carPlateData}`;
				data = await this.sendAxios(carPayUrl,"GET","");
				this.isCarPlate = false;
				
				if(!data.code){
					uni.showToast({
						title:"请求错误",
					})
					return;
				} 

				if( data.data.isAccount ){
					return;
				}
				
				this.isCarPlate = true;
				this.carOrderData = data.data;
				this.dealParkingTImer(this.carOrderData);
			},
			dealParkingTImer( timer ){
				let parkingTimer = 0;
				let dealStartData = [];
				let dealEndData = [];
				
				let startData = timer.startTimer;
				let endData = timer.endTimer;
	
				if(endData == ""){
					// 用户还未到达门口结算业务
					const time = new Date();
					const timer = formatTime(time).parkingTimer;
					const timerStr = timer.join("-");
					this.carOrderData.endTimer = timerStr;
					endData = timerStr;
				}
				
				dealStartData = startData.split("-");
				dealEndData = endData.split("-");
				
				dealStartData = dealStartData.map(Number);
				dealEndData = dealEndData.map(Number);
				
				parkingTimer = (dealEndData[2]-dealStartData[2])*24*60 + (dealEndData[3]*60 + dealEndData[4]) - (dealStartData[3]*60 + dealStartData[4]);
				this.parkingTimer = parkingTimer;
				console.log(parkingTimer);
			},
			// 结算订单
			async accountOrder(){
				const carPayUrl = `${this.accountPayUrl}?carPlate=${this.carPlateData}`;
				data = await this.sendAxios(carPayUrl,"GET","");
				if(!data.code){
					uni.showToast({
						title:"结算失败",
						icon:"error"
					})
					return;
				}
				uni.showToast({
					title:"支付成功",
					icon:"success"
				})
				return;
			}
		},
		onLoad(options) {
			if(options.carPlate){
				this.carPlateData = options.carPlate;
				this.selectOrder();
			}
		}
	}
</script>

<style>
*{
	margin: 0;
	padding: 0;
}

	
.haveCarData{
	width: 350px;
	height: 260px;
	border-radius: 5px;
	margin: 50px auto;
	box-shadow: 0 16rpx 16rpx rgba(10, 16, 20, 0.24), 0 0 16rpx rgba(10, 16, 20, 0.12);
}
.onCarData{
	width: 350px;
	height: 100px;
	border-radius: 5px;
	margin: 50px auto;
	box-shadow: 0 16rpx 16rpx rgba(10, 16, 20, 0.24), 0 0 16rpx rgba(10, 16, 20, 0.12);
}
.onCarData ul{
	width: 100%;
	height: 100%;
}
.onCarData ul li{
	width: 100%;
	height: 50%;
	text-align: center;
}
.haveCarData ul{
	width: 100%;
	height: 100%;
}

.haveCarData ul li{
	display: block;
	width: 100%;
	height: 25%;
}

.payorder{
	border-radius: 5px;
	display: flex;
	align-items: center;
	background-color: #fff;
	box-shadow: 0 16rpx 16rpx rgba(10, 16, 20, 0.24), 0 0 16rpx rgba(10, 16, 20, 0.12);
}

</style>
