package com.feifei.carInit.util;

import okhttp3.*;

import java.io.IOException;

public class AuthService {
    static final OkHttpClient HTTP_CLIENT = new OkHttpClient().newBuilder().build();

    public static String getAuth() throws IOException {
        MediaType mediaType = MediaType.parse("application/json");
        RequestBody body = RequestBody.create(mediaType, "");
        Request request = new Request.Builder()
                .url("https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=ld3ztlpkGwyg5ndNgROGGdVh&client_secret=Ntukj7GrpCW0UjGEBuHo9lBOYOAte9Eo")
                .method("POST", body)
                .addHeader("Content-Type", "application/json")
                .addHeader("Accept", "application/json")
                .build();
        Response response = HTTP_CLIENT.newCall(request).execute();
        System.out.println(response.body().string());
        return response.body().string();
    }
}
