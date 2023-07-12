import Vue from 'vue'
import axios from 'axios'

let baseURL = 'http://127.0.0.1:8001/'

const service = axios.create({
	headers: {'content-type': 'application/x-www-form-urlencoded'},
    withCredentials: true,
    crossDomain: true,
    baseURL,
    timeout: 6000
})

// request拦截器,在请求之前做一些处理
service.interceptors.request.use(
    config => {
        // if (store.state.token) {
        //     // 给请求头添加user-token
        //     config.headers["user-token"] = store.state.token;
        // }
        console.log('请求拦截成功')
        return config;
    },
    error => {
        console.log(error); // for debug
        return Promise.reject(error);
    }
);

//配置成功后的拦截器
service.interceptors.response.use(res => {
    if (res.data.status == 1) {
        return res.data
    } else {
        return Promise.reject(res.data.msg);
    }
}, error => {
    return Promise.reject(error)
})

axios.defaults.adapter = function(config) {
	return new Promise((resolve, reject) => {
		var settle = require('axios/lib/core/settle');
		var buildURL = require('axios/lib/helpers/buildURL');
		uni.request({
			method: config.method.toUpperCase(),
			url: config.baseURL + buildURL(config.url, config.params, config.paramsSerializer),
			header: config.headers,
			data: config.data,
			dataType: config.dataType,
			responseType: config.responseType,
			sslVerify: config.sslVerify,
			complete: function complete(response) {
				// console.log("执行完成：", response)
				response = {
					data: response.data,
					status: response.statusCode,
					errMsg: response.errMsg,
					header: response.header,
					config: config
				};
				settle(resolve, reject, response);
			}
		})
	})
}

export default service
