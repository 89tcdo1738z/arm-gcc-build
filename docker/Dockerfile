# =============================================================================
# docker/Dockerfile — 预构建 ARM 编译环境镜像
# =============================================================================
#
# 用途：将 arm-none-eabi-gcc 工具链打包进镜像，
#       避免每次 CI job 都要 apt-get install（节省 30~60 秒/次）。
#
# ─── 构建并推送到 Docker Hub ──────────────────────────────────────
#   cd docker
#   docker build -t yourname/arm-builder:bookworm .
#   docker push yourname/arm-builder:bookworm
#
# ─── 在 GitHub Actions 中使用自建镜像 ────────────────────────────
#   container:
#     image: yourname/arm-builder:bookworm
#   # 此时不需要 before_script 安装工具链，直接 make all
#
# ─── 在 GitLab CI 中使用自建镜像 ─────────────────────────────────
#   build-firmware:
#     image: yourname/arm-builder:bookworm
#     # 不需要 before_script 安装工具链
#     script:
#       - make all
#
# =============================================================================

FROM debian:bookworm-slim

LABEL org.opencontainers.image.title="AT32 ARM Build Environment"
LABEL org.opencontainers.image.description="arm-none-eabi-gcc + make for Cortex-M4 firmware CI"

# 安全最佳实践：单 RUN 层安装 + 清理 apt 缓存，减小镜像体积
RUN apt-get update -qq && \
    apt-get install -y -qq --no-install-recommends \
        gcc-arm-none-eabi \
        binutils-arm-none-eabi \
        make \
        git \
        ca-certificates \
    && rm -rf /var/lib/apt/lists/*   # 删除包列表缓存（减小约 30MB）

# 验证安装（构建时可见工具链版本，便于排查）
RUN arm-none-eabi-gcc --version && \
    arm-none-eabi-size --version

# CI runner 会把仓库挂载到 /workspace
WORKDIR /workspace

# 默认命令（可被 CI script 覆盖）
CMD ["make", "all"]
