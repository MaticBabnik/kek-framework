import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [vue()],
  build: {
    outDir: "../data/w"
  },
  server: {
    proxy: {
      "/api": {
        target: "http://192.168.116.102",
        changeOrigin: true,
        secure: false,
      }
    }
  }
})
