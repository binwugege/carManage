<template>
	<view class="continer">
		<uni-section title="卡片+列表" type="line">
			<uni-card padding="0" spacing="0">
				<template v-slot:cover>
					<view class="custom-cover">
						<image class="cover-image" src="../../static/1.png"
							style="width: 100%;background-repeat: no-repeat;">
						</image>
						<view class="cover-content">
							<text class="uni-subtitle uni-white">个人信息</text>
						</view>
					</view>
				</template>
				<uni-section title="个人信息" type="line">
					<uni-card :title="userInfoData.username" extra="用户账号">
						<template>
							<view><button type="primary" @click="checParking">查询停车</button></view>
						</template>
					</uni-card>
					<uni-card :title="userCarPlste" extra="车牌信息">
						<template>
							<view><button type="primary" @click="updateCarPlste">修改车牌</button></view>
						</template>
					</uni-card>
				</uni-section>
			</uni-card>
		</uni-section>
		<uni-popup ref="carPlste" type="dialog" style="width: 100%;height: 300rpx;">
			<view class="carPlste">
				<ul>
					<li><input type="text" placeholder="请输入车牌号" v-model="updateUserCarPlste" class="carInput"></li>
					<li>
						<div style="display: flex;">
							<button type="primary" @click="close"
								style="width: 40%;background-color:#fbffff;color:black;">取消</button>
							<button type="primary" @click="confirm"
								style="width: 40%;background-color:#409EFF;color: black;">确认</button>
						</div>
					</li>
				</ul>
			</view>
		</uni-popup>
		<button @click="exitLogin" style="border-top: 1px red;">退出登录</button>
		<button @click="initStorge" style="border-top: 1px red;">初始数据</button>
	</view>
</template>

<script>
	import vant_card from "../../component/uni-card/uni-card.vue"

	export default {
		data() {
			return {
				date: [{
						id: 0,
						name: '车位一',
						startNowTimer: "",
						startTimer: [0, 0],
						endTimer: [0, 0],
						status: false
					},
					{
						id: 1,
						name: '车位二',
						startNowTimer: "",
						startTimer: [0, 0],
						endTimer: [0, 0],
						status: false
					},
					{
						id: 2,
						name: '车位三',
						startNowTimer: "",
						startTimer: [0, 0],
						endTimer: [0, 0],
						status: false
					},
					{
						id: 3,
						name: '车位四',
						startNowTimer: "",
						startTimer: [0, 0],
						endTimer: [0, 0],
						status: false
					},
				],
				userInfo: {},
				userCarPlste: "",
				updateUserCarPlste: "",
				userInfoData: {},
				getUserInfo: "http://localhost:8081/user/getCarPlate",
				updateUserInfo: "http://localhost:8081/user/updateCarPlate",
			}
		},
		methods: {
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
			initStorge() {
				const arr = this.date.map((item, index) => {
					uni.setStorage({
						key: item.id + '',
						data: this.date[index],
						success(val) {
							console.log("初始化设备id:" + item.id)
						}
					});
				})
				uni.showToast({
					title: "初始化成功"
				})
			},
			async exitLogin() {
				await uni.removeStorageSync("userInfo");
				uni.redirectTo({
					url: "/pages/shezhi/shezhi"
				})
			},
			async initUser() {
				const that = this;
				uni.getStorage({
					key: "userInfo",
					success(val) {
						that.userInfoData = val.data;
						that.getCarPlste(that.userInfoData.username);
					},
					fail() {
						uni.redirectTo({
							url: "/pages/login/login"
						})
					}
				})
			},
			async getCarPlste(phone) {
				let data = {};
				const that = this;
				let carPlsteUrl = "";

				carPlsteUrl = `${that.getUserInfo}?phone=${phone}`;
				data = await that.sendAxios(carPlsteUrl, "GET", "");
				if (!data.code) {
					that.userCarPlste = "未选择车牌";
					return;
				}
				that.userCarPlste = data.data;
			},
			// 修改车牌号
			updateCarPlste() {
				this.updateUserCarPlste = this.userCarPlste;
				this.$refs.carPlste.open();
			},
			// 关闭弹窗
			close() {
				this.$refs.carPlste.close();
			},
			// 用户点击确认按钮
			async confirm() {
				let data = {};
				let carPlsteUrl = "";
				carPlsteUrl =
					`${this.updateUserInfo}?phone=${this.userInfoData.username}&carPlate=${this.updateUserCarPlste}`;
				data = await this.sendAxios(carPlsteUrl, "GET", "")
				// this.checkNumberPlate( this.updateUserCarPlste )
				console.log(data);
				this.$refs.carPlste.close();
			},
			// 判断车牌号是否正确
			checkNumberPlate(numberPlate) {
				var shortProvince = new Array("京", "沪", "津", "渝", "冀", "晋", "蒙",
					"辽", "吉", "黑", "苏", "浙", "皖", "闽", "赣", "鲁", "豫", "鄂", "湘", "粤", "桂",
					"琼", "川", "贵", "云", "藏", "陕", "甘", "青", "宁", "新");
				if (numberPlate == "" || numberPlate == null) { //验证是否为空 
					uni.showToast({
						title: "请输入车牌号",
						icon: "error"
					})
					return;
				}

				var str = numberPlate.substring(0, 1); //截取车牌号的首字符 
				var res = false;
				for (var i = 0; i < shortProvince.length; i++) { //循环判断输入的车牌号首字符是否在指定的省份内 
					if (str == shortProvince[i].toString()) {
						res = true; //在指定简写省份名称范围内 
					}
				}

				//验证普通的车牌号码 ： 吉A-E1234,吉A-EE123,吉A-12345,吉A-6A123
				var regExpression1 = /^[\u4e00-\u9fa5][a-zA-Z]-[0-9A-Za-z]{3}\d{2}$/;
				//验证车牌号尾号为字母的表达式 
				var regExpression2 = /^[\u4e00-\u9fa5][a-zA-Z]-\d{4}[a-zA-Z]$/;
				if (!regExpression1.test(numberPlate) && !regExpression2.test(numberPlate)) {
					uni.showToast({
						title: "您输入的车牌号码有误",
						icon: "error"
					})
					return;
				} else { //如果符合规则 ，判断首字符是否为各省级名称的简写 
					if (!res) {
						uni.showToast({
							title: "您输入的车牌号码有误，首字符无效！",
							icon: "error"
						})
						return;
					}
				}
			},
			checParking(){
				const that = this;
				if( this.userCarPlste != "未选择车牌"){
					uni.navigateTo({
						url:`../userpay/userpay?carPlate=${that.userCarPlste}`,
					})
				}else{
					uni.navigateTo({
						url:`../userpay/userpay?carPlate=0`,
					})
				}
			}
		},
		onLoad() {
			this.initUser();
		},
		onPullDownRefresh() {
			uni.redirectTo({
				url: "/pages/shezhi/shezhi"
			})
		}
	}
</script>

<style>
	.continer {
		width: 100%;
	}

	.carPlste {
		width: 300px;
		height: 300rpx;
		border-radius: 5px;
		background-color: white;
		box-shadow: 1px 2px 3px 1px #f7ddd9;
	}

	.carPlste ul {
		width: 100%;
		height: 100%;
	}

	.carPlste ul li:first-child {
		height: 30%;
		width: 100%;
		list-style: none;
		display: block;
		text-align: center;
		border: 2rpx solid #EEEEEE
	}

	.carPlste ul li:nth-child(2) {
		margin-top: 10%;
	}

	.carInput {
		width: 100%;
		height: 100%;
		margin-left: 25rpx;
		margin-top: 14rpx;
		color: #727272;
		font-size: 40rpx
	}
</style>
