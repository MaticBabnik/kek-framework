import { createApp } from 'vue'
import './style.less'
import App from './App.vue'
import { createRouter, createWebHashHistory } from 'vue-router'

import Home from "./views/Home.vue";
import Login from "./views/Login.vue";

const router = createRouter({
    history: createWebHashHistory("/"),
    routes: [
        {
            path: "/",
            component: Home
        },
        {
            path: "/login",
            component: Login
        }
    ]
});

createApp(App)
    .use(router)
    .mount('#app')
