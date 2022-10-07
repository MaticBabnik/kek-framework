<script lang="ts" setup>
import { computed } from '@vue/reactivity';
import { ref, watch } from 'vue';
import { useRouter } from 'vue-router';
import { useAuthStore } from './stores/authStore';
import { useKekStore } from './stores/kekStore';

const auth = useAuthStore();
const kek = useKekStore();
const router = useRouter();

const showLoadingScreen = ref<Boolean>(!!router.currentRoute.value.meta.auth && !auth.valid);

router.afterEach((to) => {
  showLoadingScreen.value = !!to.meta.auth && !auth.valid
})

function logout() {
  auth.logout();
  router.push('/login');
}

function reload() {
  kek.readAll();
}

</script>

<template>
  <div class="header">
    <span class="big">KekFw</span>
    <div style="flex:1"></div>
    <button class="button" v-if="auth.valid" @click="reload">♻</button>
    <button class="button" v-if="auth.valid" @click="logout">Logout</button>
  </div>
  <RouterView v-if="!showLoadingScreen" />
  <div id="loading" v-else>
    <h1>Loading...</h1>
  </div>
</template>

<style lang="less">
.header {
  width: 100%;
  height: 3rem;
  display: flex;
  flex-direction: row;
  background-color: #1a1a1a;
  align-items: center;

  box-sizing: border-box;
  padding: 0 .2rem;

  .big {
    color: #646cff;
    font-size: 2rem;
  }
}
</style>