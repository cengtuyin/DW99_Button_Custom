SKIPMOUNT=false
LATESTARTSERVICE=true
POSTFSDATA=false
PROPFILE=false
print_modname() {
 ui_print "*******************************"
 ui_print "     	Magisk Module        "
 ui_print "Make By 小白杨（爱玩机工具箱）"
 ui_print "*******************************"
}
on_install() {
 ui_print "- 正在释放文件"
 unzip -o "$ZIPFILE" 'main' -d $MODPATH >&2
 unzip -o "$ZIPFILE" 'system/app/com.KeyboardShortcuts.yin/com.KeyboardShortcuts.yin.apk' -d $MODPATH >&2
 unzip -o "$ZIPFILE" '脚本/双按键长按.sh' -d $MODPATH >&2
 unzip -o "$ZIPFILE" '脚本/左键单击.sh' -d $MODPATH >&2
 unzip -o "$ZIPFILE" '脚本/左键双击.sh' -d $MODPATH >&2
 unzip -o "$ZIPFILE" '脚本/左键抬起.sh' -d $MODPATH >&2
 unzip -o "$ZIPFILE" '脚本/左键按下.sh' -d $MODPATH >&2
 unzip -o "$ZIPFILE" '脚本/左键长按.sh' -d $MODPATH >&2
 unzip -o "$ZIPFILE" '脚本/电源键单击.sh' -d $MODPATH >&2
 unzip -o "$ZIPFILE" '脚本/电源键双击.sh' -d $MODPATH >&2
 unzip -o "$ZIPFILE" '脚本/电源键抬起.sh' -d $MODPATH >&2
 unzip -o "$ZIPFILE" '脚本/电源键按下.sh' -d $MODPATH >&2
 unzip -o "$ZIPFILE" '脚本/电源键长按.sh' -d $MODPATH >&2
}
set_permissions() {
 set_perm_recursive $MODPATH 0 0 0755 0644
#设置权限，基本不要去动
}