<template>
	  <view class="container">
		  <view class="login-icon">
			  <image class="login-img" src="./../../static/login/2.png"></image>
		  </view>
		  <view class="login-from">
			  <!--  账号 -->
			  <view class="inputView">
				  <image class="nameImage" src="./../../static/login/logo.png"></image>
				  <input class="inputText" v-model="userInfo.username" placeholder="请输入账号"/>
				  
			  </view>
			  <!-- 密码 -->
			  <view class="inputView">
			  	  <image class="nameImage" src="./../../static/login/1.png"></image>
			  	   <input class="inputText" v-model="userInfo.password" password="true"  placeholder="请输入密码"/>
			  </view>
			  
			  <view class="inputView">
			  	  <image class="nameImage" src="./../../static/login/1.png"></image>
			  	 <input class="inputText" v-model="userInfo.email"  placeholder="请输入邮箱"/>
			  </view>
			  <!-- 登录按钮 -->
			  <view v-if="isLogin">
				  <view class="loginBtnView" @click="handelLogin">
				  	立即登录
				  </view> 
				  <text style="display: block;text-align: right;color: blue;" @click="switchLogin">立即注册</text>
			  </view>
			  <view v-else>
				  <view class="loginBtnView" @click="handelRegister">
					立即注册
				  </view> 
				  <text style="display: block;text-align: right;color: blue;" @click="switchLogin">立即登录</text>
			  </view>
		  </view>
	  </view>
</template>
 
<script>
	// import md5Libs from "uview-ui/libs/function/md5"
	import axios from "axios";

	export default {
		data(){
			return{
				userInfo:{
					username:"",
					password:"",
					email:"",
				},	
				loginOK:0, //用户第一次登录是0，登录过后变成1，第二次以后自动登录
				isLogin:true,
			}
		},
		// 自动登录的制作
		async onLoad(){
			this.userInfo.username = await uni.getStorageSync("username")
			this.userInfo.password = await uni.getStorageSync("password")
			this.loginOK = await uni.getStorageSync("loginOK")
			if(this.loginOK === 6)
			{
				this.login()
			}
		},
		methods:{
			switchLogin(){
				this.isLogin = !this.isLogin;
			},
			async login(url){
				const username = this.userInfo.username;
				const userEmail = this.userInfo.email;
				const password = this.userInfo.password;

				if(this.userInfo.username === '')
				{
					uni.showToast({
						title:"账号不能为控",
						icon:'error'
					})
					return
				}
				
				if(this.userInfo.password === '')
				{
					uni.showToast({
						title:"密码不能为空",
						icon:'error'
					})
					return
				}
				
				var re = /^1[3,4,5,6,7,8,9][0-9]{9}$/;
				var result = re.test(this.userInfo.username); 
					if(!result) {
						uni.showToast({
							title:"手机号格式错误",
							icon:"error"
						})
						return false; //若手机号码格式不正确则返回false
				}
				this.requestData(url);
			},
			requestData(requestUrl){
				uni.request({
					url:requestUrl,
					data:this.userInfo,
					method:'POST',
					header: {
						"content-type": "application/x-www-form-urlencoded"
				    },
					success: (res) => {
						if(res.data.code){
							console.log(res.data)
							uni.setStorageSync('userInfo', {"username":this.userInfo.username,"password":this.userInfo.password,"email":this.userInfo.email})
							uni.showToast({
								icon:"success",
								title:"成功"
							})
							uni.redirectTo({
								url:"/pages/shezhi/shezhi"
							})
						 }else{
							 console.log(res.data)
							 uni.showToast({
							 	icon:"error",
								title:res.data.msg
							 })
						 }
					},
					fail: (err) => {
						console.log(err)
					}
				})
			},
			handelLogin(){
				const url = "http://127.0.0.1:8081/user/getUserInfo";
				this.login(url)
			},
			handelRegister(){
				const url = "http://127.0.0.1:8081/user/registerUser";
				this.login(url)
			}
		}
	}
</script>
 
<style>
/* #ifndef H5 */
page {
	height: 100%;
	background-color: #eeeeee;
}
/* #endif */
</style>
 
<style lang="scss" scoped>
	page{ 
	 height: 100%; 
	} 
	  
	.container { 
	 height: 100%; 
	 display: flex; 
	 flex-direction: column; 
	 padding: 0; 
	 box-sizing: border-box; 
	 background-color: #f2f6fc;
	   /*登录图片*/
	   .login-icon{ 
	     flex: none; 
	   } 
	   .login-img{ 
	    width: 750rpx; 
	   } 
	   
	   /*表单内容*/
	   .login-from view:nth-child(2){
	   			  margin-top: 42rpx;
	   }
	   .login-from { 
	    margin-top: -185rpx; 
		margin-left: 37rpx;
	    position: relative;
	    height:495rpx;
		width: 676rpx;
		background: #ffffff;
		border-radius: 6rpx;
		.inputView {
		   background-color: #FFFFFF; 
		   margin-top: 72rpx;
		   margin-left: 32rpx;
		   display: flex;
		   border-radius: 6rpx;
		   height: 76rpx;
		   border: 2rpx solid #EEEEEE ;
		   width: 616rpx;
		        /*输入框*/
			   .nameImage{ 
				margin-left: 22rpx; 
				margin-top: 22rpx;
				width: 24rpx; 
				height: 24rpx
			   }  
			   .inputText {
				margin-left: 25rpx; 
				margin-top: 14rpx;
				color: #727272; 
				font-size: 30rpx
			   }  
		  } 
		  .loginBtnView{
			  margin-top: 42rpx;
			  margin-left:24rpx;
			  background: #5f8bd2;
			  padding-top: 20rpx;
			  text-align: center;
			  font-size: 32rpx;
			  letter-spacing:4rpx;
			  color: #f5ffed;
			  height: 78rpx;
			  width: 639rpx;
			  border-radius: 32rpx;
			  
		  }
		  
		     
	   } 
	} 
</style>
