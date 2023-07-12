<template>
	<view>
		<view class="main_continue">
			<view v-if="userBookingData" class="continue" v-for="(item,index) in userBookingData" :key="index">
				<div class="msg_left">
					<div class="msg_left_top"></div>
					<div class="msg_left_bottom">
						<span class="data">设备ID:{{item.id}}</span>
						<span class="data">设备名:{{item.name}}</span>
					</div>
				</div>
				<div class="msg_right">
					<div class="msg_right_top">
						<span style="display: block;margin:20px 0 10px 0;color: red;">设备预约状态</span>
						<switch disabled="true" :checked="item.booking" name="switch" />
						<text>预约中</text>
					</div>
					<div class="msg_right_bottom">
						<button type="primary" @click="cancelBooking(item)" size="mini">取消预约</button>
					</div>
					
				</div>
		
			</view>
			<view v-else class="nullBooking">
				您还没有预约哦....
			</view>
		</view>
	</view>
</template>

<script>
	import mqtt from './../../utils/mqtt.js'
	const aliyunOpt = require("../../utils/aliyun/aliyun_connect.js");
	
	export default {
		data() {
			return {
				bookIng:[
					{id:0,name:"车位一",booking:true},
					{id:1,name:"车位二",booking:false},
					{id:2,name:"车位三",booking:false},
					{id:3,name:"车位四",booking:true}
				],
				userBookingData:[],
				userInfo:{username:"",password:"",email:""},
				getBookingUrl:"http://127.0.0.1:8081/car/getUserBooking?",
				getUserDataUrl:"http://127.0.0.1:8081/user/getUserInfo",
				cancelBookingUrl:"http://127.0.0.1:8081/car/cancel?",
				userLoginStatus:false,
			}
		},
		methods: {
			async cancelBooking(item){
				let cancelBooingStatus = {};
				await this.dealLoginUser();
				const url = `${this.cancelBookingUrl}id=${item.id}&phone=${this.userInfo.username}`
				cancelBooingStatus =  await this.requestData(url,"GET",{})
				console.log(url)
				if(cancelBooingStatus.code){
					uni.showToast({
						title:"取消成功",
						icon:"success"
					})
					uni.redirectTo({
						url:"/pages/userbook/userbook"
					})
				}else{
					uni.showToast({
						title:cancelBooingStatus.msg,
						icon:"error"
					})
				}
			},
			requestData(url,method,data){		
				const that = this;
				return new Promise((resolve,reject)=>{
					uni.request({
						url:url,
						method:method,
						data:data,
						header: {
							"content-type": "application/x-www-form-urlencoded"
						},
						success(res) {
							resolve(res.data);
						},
						fail() {
							reject(0);
						}
					})
				})
			},
			// 获取用户的预约车位信息
			async getUserBooking(){
				let userData = {};
				this.userBookingData = [];
				let url = this.getBookingUrl + "phone=" + this.userInfo.username
				userData = await this.requestData(url,"GET",{});
				this.userBookingData = userData.data;
				console.log(url)
			},
			// 判断用户是否登录
			async dealLoginUser(){
				const data = await uni.getStorageSync("userInfo");
				if(data == ""){
					this.userLoginStatus = false;
					uni.showToast({
						title:"请先登录",
						icon:"error"
					})
					uni.redirectTo({
						url:"/pages/login/login"
					})
					return;
				}else{
					this.userInfo = data;
					const userStatus = await this.requestData(this.getUserDataUrl,"POST",data);
					if(userStatus.code){
						this.userLoginStatus = true;
					}else{
						uni.showToast({
							title:"用户信息错误",
							icon:"error"
						})
					}
					
				}
				
			},
			// 计入阿里云
			connectAliYun(){
					// clearTimeout(time);
					const that = this;
					const carData = this.date;	
					let clientOpt = aliyunOpt.getAliyunIotMqttClient({
					  productKey: that.data.aliyunInfo.productKey,
					  deviceName: that.data.aliyunInfo.deviceName,
					  deviceSecret: that.data.aliyunInfo.deviceSecret,
					  regionId: that.data.aliyunInfo.regionId,
					  port: that.data.aliyunInfo.port,
					});
					
					console.log(1111)		
					let host = 'wxs://' + clientOpt.host;
					this.data.options.clientId = clientOpt.clientId;
					this.data.options.password = clientOpt.password;
					this.data.options.username = clientOpt.username;
					this.data.client = mqtt.connect(host, this.data.options);		
					this.data.client = mqtt.connect(host, this.data.options);		
					this.data.client.on('connect', function (connack) {
						wx.showToast({
							title: '连接成功'
						})
						console.log("连接成功");
					})
				}
		},
		async onLoad() {
			await this.dealLoginUser();
			await this.getUserBooking();
			
		},
		async onPullDownRefresh() {
			await this.dealLoginUser();
			await this.getUserBooking();
			
		}
	}
</script>

<style>
	.main_continue{
		width: 100%;
		height: 150px;
		background-color: #fcfcfc;
		display: flex;
		justify-content: center;
		flex-wrap: wrap;
	}
	.continue{
		display: block;
		width: 90%;
		height: 100%;
		margin-top: 20px;
		background-color: #fff;
		border-radius: 20px;
		box-shadow: 1px 2px 3px 4px #fff4f7;
		display: flex;
		justify-content: center;
	}
	.data{
		display: block;
		text-align: center;
		margin-top: 4px;
		color: #000000;
	}
	.msg_left{
		width: 50%;
		height: 100%;
	}
	.msg_right{
		width: 50%;
		height: 100%;
	}
	.msg_left_top{
		width: 100%;
		height: 65%;
		background-size: 50% 90%;
		border-radius: 50%;
		background-image: url(../../static/car.png);
		background-repeat: no-repeat;
		background-position: center;
	}
	
	.msg_right_top{
		width: 100%;
		height: 50%;
	}
	.msg_right_bottom{
		margin-top: 15rpx;
		width: 100%;
		height: 50%;
	}
	.nullBooking{
		margin-top: 200px;
	}
</style>

