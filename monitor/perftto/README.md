### 性能分析工具


- 说明网址：
https://perfetto.dev/docs/quickstart/linux-tracing   


- 使用说明

```shell
# 获取代码
git clone https://android.googlesource.com/platform/external/perfetto/ && cd perfetto  

# 进入工具目录
tools/install-build-deps

# 生成工具
tools/gn gen --args='is_debug=false' out/linux
# Or use `tools/setup_all_configs.py` to generate more build configs. 


# 生成工具 tracebox、traced traced_probes、perfetto、trace_processor_shell  
tools/ninja -C out/linux tracebox traced traced_probes perfetto trace_processor_shell
```
