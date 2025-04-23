# 2025-04-16T08:58:46.432012600
import vitis

client = vitis.create_client()
client.set_workspace(path="project_22_ddr3")

comp = client.create_app_component(name="hello_world",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_microblaze_riscv_0",template = "hello_world")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="hello_world")
comp.build()

vitis.dispose()

client.delete_component(name="hello_world")

status = platform.build()

comp = client.get_component(name="gpstest")
comp.build()

status = platform.build()

comp.build()

