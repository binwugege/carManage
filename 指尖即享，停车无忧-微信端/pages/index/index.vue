<template>
	<view class="content">
		<swiper style="height: 305rpx;width: 100%;" class="swiper" circular :indicator-dots="indicatorDots" :autoplay="autoplay" :interval="interval"
			:duration="duration">
			<swiper-item>
				<view class="swiper-item uni-bg-red" style="background-image: url(../../static/1.png);background-size: 100% 100%;width: 100%; height:100%;"></view>
			</swiper-item>
			<swiper-item>
				<view class="swiper-item uni-bg-green" style="background-image: url(../../static/1.png);background-size: 100% 100%;width: 100%; height:100%;"></view>
			</swiper-item>
			<swiper-item>
				<view class="swiper-item uni-bg-blue"  style="background-image: url(../../static/1.png);background-size: 100% 100%;width: 100%; height:100%;"></view>
			</swiper-item>
		</swiper>
		<view class="car_nav">
			<ul>
				<li @click="swithTab">
					<div class="nav_img"></div>
					<span class="nav_text">预约</span>
				</li>
				<li @click="accountOrder">
					<div class="nav_img"></div>
					<span class="nav_text">结算</span>
				</li>
				<li @click="swithTab1">
					<div class="nav_img"></div>
					<span class="nav_text">关于</span>
				</li>
				<li @click="swithTab1"> 
					<div class="nav_img"></div>
					<span class="nav_text">电话</span>
				</li>
			</ul>
		</view>
		<car :origin_carDate="date" :pull="pull1"></car>
		<uni-popup ref="popupText" type="dialog">
			<view class="diag_text">
				欢迎使用停车管理系统
			</view>
		</uni-popup>
	</view>
</template>

<script>
	import mqtt, { log } from './../../utils/mqtt.js'
	import car from './../../component/desk.vue'
	const aliyunOpt = require("../../utils/aliyun/aliyun_connect.js");
	
	export default {
		data() {
			return {
				date:[
					{id:0,name:'车位一',startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:false},
					{id:1,name:'车位二',startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:false},
					{id:2,name:'车位三',startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:false},
					{id:3,name:'车位四',startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:false},
				],
				pull1:0,
				background: ['color1', 'color2', 'color3'],
				
			}
		},
		components:{
			car
		},
		onShow(){
			const that = this;
			const carDa = this.date;
			carDa.map((item,i)=>{
				uni.getStorage({
					key:i + '',
					success(val) {
						carDa[i] = val.data;
					},
					fail() {
						uni.setStorage({
							key:item,
							data:carDa[i],
							success() {
								console.log("初始化数据成功");
							}
						})
					}
				})
				that.pull1 = 1; 	
			})
			/*
			this.data.client.on("message", function (topic, payload) {
				  console.log(" 收到 topic:" + topic + " , payload :" + payload);
					try{
						const data = JSON.parse(payload).led;
						const dealData = data.split("_");
						uni.setStorage({
							key:dealData[0],
							data:dealData[1]
						})
						console.log(dealData);
					}catch(e){
						console.log('订阅消息的数据格式有误')
					}
				})
				*/
		},
		methods: {
			subTopic:function(){
				this.data.client.publish(this.data.aliyunInfo.pubTopic,`{"data":${this.data.test}}`);
				wx.showToast({title:"发送成功"})
			},changeIndicatorDots(e) {
				this.indicatorDots = !this.indicatorDots
			},
			changeAutoplay(e) {
				this.autoplay = !this.autoplay
			},
			intervalChange(e) {
				this.interval = e.target.value
			},
			durationChange(e) {
				this.duration = e.target.value
			},
			swithTab1(){
				uni.switchTab({
					url:'../message/message'
				})
			},
			swithTab(){
				const data = JSON.stringify(this.date);
				console.log(data)
				uni.navigateTo({
					url:`/pages/detail/detail?data=${data}`
				})
			},
			getCachesData(){
				const that = this;
				const carDa = this.date;
				carDa.map((item,i)=>{
					uni.getStorage({
						key:i + '',
						success(val) {
							that.date[i] = val.data;
						},
						fail() {
						uni.setStorage({
							key:i +'',
							data:carDa[1],
							success() {
								console.log("初始化数据成功");
							}
						})
					}
					})
				})
				this.pull1 = !this.pull1;
				console.log(this.pull1) 
				
			},
			// 想要结算订单
			accountOrder(){
				uni.navigateTo({
					url:"./../userpay/userpay"
				})
			}
		},
		onLoad(){
			let that = this;
			let carDa = this.date;
			uni.$on("unpdateDataBooking",function(){
				that.pull1 = !that.pull1;
			});
			this.getCachesData();
			// this.$refs.popupText.open(); // 开启弹窗提示	
		},
		onPullDownRefresh() {
			wx.showToast({
				title: '刷新中'
			})
			this.getCachesData()
			const timer1 = setTimeout(()=>{
				uni.stopPullDownRefresh();
				wx.showToast({
					title: '刷新成功'
				})
			},1000)
			
		}
	}
</script>

<style style="less">
	@import url("http//at.alicdn.com/t/c/font_3867040_s2t6jthrs19.css")
	
	.content{
		width: 100%;
		height: 100%;
		background-color: #ffffff;
	}
	.car_nav{
		width: 100%;
		height: 80px;
		border-right-color: red;
	}
	.car_nav ul{
		display: flex;
		width: 100%;
		height: 100%;
		justify-content: space-around;
		align-items: center;
		box-shadow: 1px 2px 3px 4px #f1f1f1;
	}
	.car_nav ul li{
		width: 20%;
		height: 80%;
		margin-top: 10px;
		display: block;
		text-decoration: none;
		text-align: center;
	}
	.car_nav ul li .nav_img{
		width: 100%;
		height: 60%;
		background-color: #fff;
		background-image: url(../../static/nav/iphone.png);
		background-size: 80% 120%;
		background-position: center;
	}
	.car_nav ul li span{
		margin-top: 5px;
		font-size: 12px;
		display: block;
	}
	.car_nav ul li:nth-child(1) .nav_img{
		width: 100%;
		height: 60%;
		background-color: #fff;
		background-image: url(../../static/nav/yuyue.png);
		background-size: 40% 50%;
		background-repeat: no-repeat;
		background-position: center;
	}
	.car_nav ul li:nth-child(2) .nav_img{
		background-image: url(../../static/li/shezhi.png);
		background-size: 40% 50%;
		background-repeat: no-repeat;
		background-position: center;
	}
	.car_nav ul li:nth-child(3) .nav_img{
		background-image: url(../../static/li/guanyu.png);
		background-size: 35% 50%;
		background-repeat: no-repeat;
		background-position: center;
	}
	.diag_text{
		width: 300px;
		height: 300px;
		text-align: center;
		line-height: 300px;
		background-color: #fff;
		border-radius: 10px;
		box-shadow: 1px 2px 3px 4px #cccccc;
	}
	.nav_text{
		color: #555555;
	}
</style>
