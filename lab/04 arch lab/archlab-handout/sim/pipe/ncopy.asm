
./a.out:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 2f 00 00 	mov    0x2fd9(%rip),%rax        # 3fe8 <__gmon_start__@Base>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	ret    

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 92 2f 00 00    	push   0x2f92(%rip)        # 3fb8 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	f2 ff 25 93 2f 00 00 	bnd jmp *0x2f93(%rip)        # 3fc0 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:	0f 1f 00             	nopl   (%rax)
    1030:	f3 0f 1e fa          	endbr64 
    1034:	68 00 00 00 00       	push   $0x0
    1039:	f2 e9 e1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    103f:	90                   	nop
    1040:	f3 0f 1e fa          	endbr64 
    1044:	68 01 00 00 00       	push   $0x1
    1049:	f2 e9 d1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    104f:	90                   	nop

Disassembly of section .plt.got:

0000000000001050 <__cxa_finalize@plt>:
    1050:	f3 0f 1e fa          	endbr64 
    1054:	f2 ff 25 9d 2f 00 00 	bnd jmp *0x2f9d(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    105b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .plt.sec:

0000000000001060 <__printf_chk@plt>:
    1060:	f3 0f 1e fa          	endbr64 
    1064:	f2 ff 25 5d 2f 00 00 	bnd jmp *0x2f5d(%rip)        # 3fc8 <__printf_chk@GLIBC_2.3.4>
    106b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001070 <exit@plt>:
    1070:	f3 0f 1e fa          	endbr64 
    1074:	f2 ff 25 55 2f 00 00 	bnd jmp *0x2f55(%rip)        # 3fd0 <exit@GLIBC_2.2.5>
    107b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .text:

0000000000001080 <main>:
    1080:	f3 0f 1e fa          	endbr64 
    1084:	50                   	push   %rax
    1085:	58                   	pop    %rax
    1086:	bf 08 00 00 00       	mov    $0x8,%edi
    108b:	48 83 ec 08          	sub    $0x8,%rsp
    108f:	48 8b 05 8a 2f 00 00 	mov    0x2f8a(%rip),%rax        # 4020 <src>
    1096:	66 0f 6f 05 72 0f 00 	movdqa 0xf72(%rip),%xmm0        # 2010 <_IO_stdin_used+0x10>
    109d:	00 
    109e:	0f 11 00             	movups %xmm0,(%rax)
    10a1:	66 0f 6f 05 77 0f 00 	movdqa 0xf77(%rip),%xmm0        # 2020 <_IO_stdin_used+0x20>
    10a8:	00 
    10a9:	0f 11 40 10          	movups %xmm0,0x10(%rax)
    10ad:	e8 0e 01 00 00       	call   11c0 <ncopy>
    10b2:	bf 01 00 00 00       	mov    $0x1,%edi
    10b7:	48 8d 35 46 0f 00 00 	lea    0xf46(%rip),%rsi        # 2004 <_IO_stdin_used+0x4>
    10be:	89 c2                	mov    %eax,%edx
    10c0:	31 c0                	xor    %eax,%eax
    10c2:	e8 99 ff ff ff       	call   1060 <__printf_chk@plt>
    10c7:	31 ff                	xor    %edi,%edi
    10c9:	e8 a2 ff ff ff       	call   1070 <exit@plt>
    10ce:	66 90                	xchg   %ax,%ax

00000000000010d0 <_start>:
    10d0:	f3 0f 1e fa          	endbr64 
    10d4:	31 ed                	xor    %ebp,%ebp
    10d6:	49 89 d1             	mov    %rdx,%r9
    10d9:	5e                   	pop    %rsi
    10da:	48 89 e2             	mov    %rsp,%rdx
    10dd:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    10e1:	50                   	push   %rax
    10e2:	54                   	push   %rsp
    10e3:	45 31 c0             	xor    %r8d,%r8d
    10e6:	31 c9                	xor    %ecx,%ecx
    10e8:	48 8d 3d 91 ff ff ff 	lea    -0x6f(%rip),%rdi        # 1080 <main>
    10ef:	ff 15 e3 2e 00 00    	call   *0x2ee3(%rip)        # 3fd8 <__libc_start_main@GLIBC_2.34>
    10f5:	f4                   	hlt    
    10f6:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    10fd:	00 00 00 

0000000000001100 <deregister_tm_clones>:
    1100:	48 8d 3d 09 2f 00 00 	lea    0x2f09(%rip),%rdi        # 4010 <__TMC_END__>
    1107:	48 8d 05 02 2f 00 00 	lea    0x2f02(%rip),%rax        # 4010 <__TMC_END__>
    110e:	48 39 f8             	cmp    %rdi,%rax
    1111:	74 15                	je     1128 <deregister_tm_clones+0x28>
    1113:	48 8b 05 c6 2e 00 00 	mov    0x2ec6(%rip),%rax        # 3fe0 <_ITM_deregisterTMCloneTable@Base>
    111a:	48 85 c0             	test   %rax,%rax
    111d:	74 09                	je     1128 <deregister_tm_clones+0x28>
    111f:	ff e0                	jmp    *%rax
    1121:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1128:	c3                   	ret    
    1129:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001130 <register_tm_clones>:
    1130:	48 8d 3d d9 2e 00 00 	lea    0x2ed9(%rip),%rdi        # 4010 <__TMC_END__>
    1137:	48 8d 35 d2 2e 00 00 	lea    0x2ed2(%rip),%rsi        # 4010 <__TMC_END__>
    113e:	48 29 fe             	sub    %rdi,%rsi
    1141:	48 89 f0             	mov    %rsi,%rax
    1144:	48 c1 ee 3f          	shr    $0x3f,%rsi
    1148:	48 c1 f8 03          	sar    $0x3,%rax
    114c:	48 01 c6             	add    %rax,%rsi
    114f:	48 d1 fe             	sar    %rsi
    1152:	74 14                	je     1168 <register_tm_clones+0x38>
    1154:	48 8b 05 95 2e 00 00 	mov    0x2e95(%rip),%rax        # 3ff0 <_ITM_registerTMCloneTable@Base>
    115b:	48 85 c0             	test   %rax,%rax
    115e:	74 08                	je     1168 <register_tm_clones+0x38>
    1160:	ff e0                	jmp    *%rax
    1162:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    1168:	c3                   	ret    
    1169:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001170 <__do_global_dtors_aux>:
    1170:	f3 0f 1e fa          	endbr64 
    1174:	80 3d 95 2e 00 00 00 	cmpb   $0x0,0x2e95(%rip)        # 4010 <__TMC_END__>
    117b:	75 2b                	jne    11a8 <__do_global_dtors_aux+0x38>
    117d:	55                   	push   %rbp
    117e:	48 83 3d 72 2e 00 00 	cmpq   $0x0,0x2e72(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    1185:	00 
    1186:	48 89 e5             	mov    %rsp,%rbp
    1189:	74 0c                	je     1197 <__do_global_dtors_aux+0x27>
    118b:	48 8b 3d 76 2e 00 00 	mov    0x2e76(%rip),%rdi        # 4008 <__dso_handle>
    1192:	e8 b9 fe ff ff       	call   1050 <__cxa_finalize@plt>
    1197:	e8 64 ff ff ff       	call   1100 <deregister_tm_clones>
    119c:	c6 05 6d 2e 00 00 01 	movb   $0x1,0x2e6d(%rip)        # 4010 <__TMC_END__>
    11a3:	5d                   	pop    %rbp
    11a4:	c3                   	ret    
    11a5:	0f 1f 00             	nopl   (%rax)
    11a8:	c3                   	ret    
    11a9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000011b0 <frame_dummy>:
    11b0:	f3 0f 1e fa          	endbr64 
    11b4:	e9 77 ff ff ff       	jmp    1130 <register_tm_clones>
    11b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000011c0 <ncopy>:
    11c0:	f3 0f 1e fa          	endbr64 
    11c4:	83 ff 03             	cmp    $0x3,%edi
    11c7:	7e 71                	jle    123a <ncopy+0x7a>
    11c9:	48 8b 0d 50 2e 00 00 	mov    0x2e50(%rip),%rcx        # 4020 <src>
    11d0:	48 8b 35 41 2e 00 00 	mov    0x2e41(%rip),%rsi        # 4018 <dst>
    11d7:	48 8d 41 0f          	lea    0xf(%rcx),%rax
    11db:	48 29 f0             	sub    %rsi,%rax
    11de:	48 83 f8 1e          	cmp    $0x1e,%rax
    11e2:	0f 86 e8 00 00 00    	jbe    12d0 <ncopy+0x110>
    11e8:	83 ef 04             	sub    $0x4,%edi
    11eb:	31 c0                	xor    %eax,%eax
    11ed:	31 d2                	xor    %edx,%edx
    11ef:	41 89 f8             	mov    %edi,%r8d
    11f2:	41 c1 e8 02          	shr    $0x2,%r8d
    11f6:	41 83 c0 01          	add    $0x1,%r8d
    11fa:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    1200:	f3 0f 6f 04 01       	movdqu (%rcx,%rax,1),%xmm0
    1205:	83 c2 01             	add    $0x1,%edx
    1208:	0f 11 04 06          	movups %xmm0,(%rsi,%rax,1)
    120c:	48 83 c0 10          	add    $0x10,%rax
    1210:	41 39 d0             	cmp    %edx,%r8d
    1213:	77 eb                	ja     1200 <ncopy+0x40>
    1215:	89 fa                	mov    %edi,%edx
    1217:	c1 ea 02             	shr    $0x2,%edx
    121a:	8d 42 01             	lea    0x1(%rdx),%eax
    121d:	f7 da                	neg    %edx
    121f:	48 c1 e0 04          	shl    $0x4,%rax
    1223:	8d 3c 97             	lea    (%rdi,%rdx,4),%edi
    1226:	48 01 c1             	add    %rax,%rcx
    1229:	48 01 c6             	add    %rax,%rsi
    122c:	48 89 0d ed 2d 00 00 	mov    %rcx,0x2ded(%rip)        # 4020 <src>
    1233:	48 89 35 de 2d 00 00 	mov    %rsi,0x2dde(%rip)        # 4018 <dst>
    123a:	83 ff 02             	cmp    $0x2,%edi
    123d:	74 11                	je     1250 <ncopy+0x90>
    123f:	83 ff 03             	cmp    $0x3,%edi
    1242:	74 6c                	je     12b0 <ncopy+0xf0>
    1244:	83 ff 01             	cmp    $0x1,%edi
    1247:	74 3f                	je     1288 <ncopy+0xc8>
    1249:	31 c0                	xor    %eax,%eax
    124b:	c3                   	ret    
    124c:	0f 1f 40 00          	nopl   0x0(%rax)
    1250:	48 8b 15 c9 2d 00 00 	mov    0x2dc9(%rip),%rdx        # 4020 <src>
    1257:	48 8b 0d ba 2d 00 00 	mov    0x2dba(%rip),%rcx        # 4018 <dst>
    125e:	31 c0                	xor    %eax,%eax
    1260:	8b 72 04             	mov    0x4(%rdx),%esi
    1263:	85 f6                	test   %esi,%esi
    1265:	89 71 04             	mov    %esi,0x4(%rcx)
    1268:	8b 12                	mov    (%rdx),%edx
    126a:	40 0f 9f c6          	setg   %sil
    126e:	40 0f b6 f6          	movzbl %sil,%esi
    1272:	89 11                	mov    %edx,(%rcx)
    1274:	01 f0                	add    %esi,%eax
    1276:	85 d2                	test   %edx,%edx
    1278:	0f 9f c2             	setg   %dl
    127b:	0f b6 d2             	movzbl %dl,%edx
    127e:	01 d0                	add    %edx,%eax
    1280:	c3                   	ret    
    1281:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1288:	48 8b 15 91 2d 00 00 	mov    0x2d91(%rip),%rdx        # 4020 <src>
    128f:	48 8b 0d 82 2d 00 00 	mov    0x2d82(%rip),%rcx        # 4018 <dst>
    1296:	31 c0                	xor    %eax,%eax
    1298:	8b 12                	mov    (%rdx),%edx
    129a:	85 d2                	test   %edx,%edx
    129c:	89 11                	mov    %edx,(%rcx)
    129e:	0f 9f c2             	setg   %dl
    12a1:	0f b6 d2             	movzbl %dl,%edx
    12a4:	01 d0                	add    %edx,%eax
    12a6:	c3                   	ret    
    12a7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
    12ae:	00 00 
    12b0:	48 8b 15 69 2d 00 00 	mov    0x2d69(%rip),%rdx        # 4020 <src>
    12b7:	48 8b 0d 5a 2d 00 00 	mov    0x2d5a(%rip),%rcx        # 4018 <dst>
    12be:	8b 42 08             	mov    0x8(%rdx),%eax
    12c1:	85 c0                	test   %eax,%eax
    12c3:	89 41 08             	mov    %eax,0x8(%rcx)
    12c6:	0f 9f c0             	setg   %al
    12c9:	0f b6 c0             	movzbl %al,%eax
    12cc:	eb 92                	jmp    1260 <ncopy+0xa0>
    12ce:	66 90                	xchg   %ax,%ax
    12d0:	83 ef 04             	sub    $0x4,%edi
    12d3:	48 89 c8             	mov    %rcx,%rax
    12d6:	48 89 f2             	mov    %rsi,%rdx
    12d9:	41 89 f8             	mov    %edi,%r8d
    12dc:	41 c1 e8 02          	shr    $0x2,%r8d
    12e0:	49 c1 e0 04          	shl    $0x4,%r8
    12e4:	4e 8d 4c 01 10       	lea    0x10(%rcx,%r8,1),%r9
    12e9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    12f0:	44 8b 00             	mov    (%rax),%r8d
    12f3:	48 83 c0 10          	add    $0x10,%rax
    12f7:	48 83 c2 10          	add    $0x10,%rdx
    12fb:	44 89 42 f0          	mov    %r8d,-0x10(%rdx)
    12ff:	44 8b 40 f4          	mov    -0xc(%rax),%r8d
    1303:	44 89 42 f4          	mov    %r8d,-0xc(%rdx)
    1307:	44 8b 40 f8          	mov    -0x8(%rax),%r8d
    130b:	44 89 42 f8          	mov    %r8d,-0x8(%rdx)
    130f:	44 8b 40 fc          	mov    -0x4(%rax),%r8d
    1313:	44 89 42 fc          	mov    %r8d,-0x4(%rdx)
    1317:	4c 39 c8             	cmp    %r9,%rax
    131a:	75 d4                	jne    12f0 <ncopy+0x130>
    131c:	e9 f4 fe ff ff       	jmp    1215 <ncopy+0x55>

Disassembly of section .fini:

0000000000001324 <_fini>:
    1324:	f3 0f 1e fa          	endbr64 
    1328:	48 83 ec 08          	sub    $0x8,%rsp
    132c:	48 83 c4 08          	add    $0x8,%rsp
    1330:	c3                   	ret    
