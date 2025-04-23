# 2025-04-16T14:39:49.055499100
import vitis

client = vitis.create_client()
client.set_workspace(path="project_22_ddr3")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="gpstest")
comp.build()

comp = client.create_app_component(name="hello_world",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_microblaze_riscv_0",template = "hello_world")

client.delete_component(name="hello_world")

